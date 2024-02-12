using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Net;
using System.Threading;
using Microsoft.Win32;
using Newtonsoft.Json;

namespace MWSE
{
	/// <summary>
	/// The main auto updater program. The program will do the following:
	/// 1) Reads the GitHub API to find the commit hash for the latest release.
	/// 2) Compares its information to the local mwse-version.txt file.
	/// 3) If the version doesn't match, it downloads the file from https://github.com/MWSE/MWSE/releases/download/build-automatic/mwse.zip
	/// 4) Extracts the contents and cleans up after itself.
	/// 5) Saves the new version as mwse-version.txt.
	/// </summary>
	class AutoUpdater
	{
		/// <summary>
		/// Where we download the file from.
		/// </summary>
		static readonly string DownloadURL = "https://github.com/MWSE/MWSE/releases/download/build-automatic/mwse.zip";
		static readonly string GitHubVersionInfoURL = "https://api.github.com/repos/MWSE/MWSE/releases/tags/build-automatic";

		/// <summary>
		/// Files that are in use by the updater, and so need to be extracted as temporary files.
		/// These files are then swapped out when MWSE is loaded by running Morrowind.exe.
		/// </summary>
		static readonly HashSet<string> TemporaryInstallFiles = new HashSet<string> { "MWSE-Update.exe", "Newtonsoft.Json.dll" };

		/// <summary>
		/// Determines if Morrowind is currently running in the background.
		/// </summary>
		/// <returns>
		/// True if the process could be found, otherwise false.
		/// </returns>
		static bool AreGameExecutablesRunning()
		{
			foreach (Process process in Process.GetProcesses())
			{
				if (process.ProcessName.Contains("Morrowind") || process.ProcessName.Contains("TES Construction Set"))
				{
					return true;
				}
			}

			return false;
		}

		/// <summary>
		/// Fetches a remote URL json's content, and attempts to interpret it as a C# object.
		/// </summary>
		/// <typeparam name="T">The object to attempt to convert to.</typeparam>
		/// <param name="url">The URL to fetch the json from.</param>
		/// <returns></returns>
		private static T DownloadSerializedJsonData<T>(string url) where T : new()
		{
			HttpWebRequest request = WebRequest.Create(url) as HttpWebRequest;
			request.Method = "GET";
			request.Proxy = null;
			request.PreAuthenticate = true;
			request.UserAgent = "MWSE-Updater";

			var json = string.Empty;
			using (HttpWebResponse response = request.GetResponse() as HttpWebResponse)
			{
				using (StreamReader reader = new StreamReader(response.GetResponseStream()))
				{
					json = reader.ReadToEnd();
				}

			}

			return !string.IsNullOrEmpty(json) ? JsonConvert.DeserializeObject<T>(json) : new T();
		}

		/// <summary>
		/// Determines if the MO2 VFS module is loaded by the current process.
		/// </summary>
		/// <returns>True if the MO2 VFS is loaded.</returns>
		static bool IsLaunchedThroughMO2()
		{
			foreach (ProcessModule module in Process.GetCurrentProcess().Modules)
			{
				if (module.ModuleName == "usvfs_x86.dll")
				{
					return true;
				}
			}

			return false;
		}

		/// <summary>
		/// Deletes a folder if it exists. Because the original function is apparently too dumb.
		/// </summary>
		/// <param name="path">The path to delete.</param>
		/// <param name="recursive">If true, subfolders will also be deleted. As they should be.</param>
		static void DeleteFolderIfExists(string path, bool recursive)
		{
			if (Directory.Exists(path))
			{
				Directory.Delete(path, recursive);
			}
		}

		/// <summary>
		/// Deletes a file if it exists. Because the original function is apparently too dumb.
		/// </summary>
		/// <param name="path">The path to delete.</param>
		static void DeleteFileIfExists(string path)
		{
			if (File.Exists(path))
			{
				File.Delete(path);
			}
		}

		/// <summary>
		/// Program entry point.
		/// </summary>
		/// <param name="args">
		/// Arguments. Currently none supported.
		/// </param>
		static int Main(string[] args)
		{
			// Prevent updating through MO2.
			if (IsLaunchedThroughMO2())
			{
				Console.WriteLine("Updating MWSE through MO2 can lead to duplicate core files, out of date files, and has caused support headaches in the past. It is no longer supported.");
				Console.WriteLine("Press any key to exit...");
				Console.ReadKey();
				return 1;
			}

			// Check to see if Morrowind is running; wait for it to close.
			if (AreGameExecutablesRunning())
			{
				Console.WriteLine("Waiting for Morrowind and the Construction Set to close...");
				while (AreGameExecutablesRunning())
				{
					Thread.Sleep(1000);
				}
			}

			// Check to see if we want to start the game after checking for updates.
			bool startAfter = false;
			if (args.Contains("-startAfter"))
			{
				startAfter = true;
			}

			// Check to see if we want to specifically overwrite existing non-script files.
			bool overwriteResources = false;
			if (args.Contains("-overwriteResources"))
			{
				overwriteResources = true;
			}

			// Try to find Morrowind's install location, store it here.
			String installLocation = null;

			// First we'll check the current directory.
			if (File.Exists("Morrowind.exe"))
			{
				installLocation = AppDomain.CurrentDomain.BaseDirectory;
			}
			// Fall back to the registry.
			else if (String.IsNullOrEmpty(installLocation))
			{
				String registryValue = Registry.GetValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\bethesda softworks\\Morrowind", "Installed Path", null) as String;
				if (!String.IsNullOrEmpty(registryValue) && File.Exists(Path.Combine(registryValue, "Morrowind.exe")))
				{
					installLocation = registryValue;
				}
			}

			// Make sure that we actually have a valid install location.
			if (!String.IsNullOrEmpty(installLocation))
			{
				Console.WriteLine("Found install location: {0}", installLocation);
			}
			else
			{
				Console.WriteLine("ERROR: Could not determine Morrowind install location. Please place the program in the same folder as Morrowind.exe.");
#if _DEBUG
                Console.ReadKey();
#endif
				return 1;
			}

			try
			{
				// On Windows versions before 8.1, we have to force TLS support.
				if (Environment.OSVersion.Platform == PlatformID.Win32NT && Environment.OSVersion.Version.CompareTo(new Version(6, 3)) < 0)
				{
					ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls | SecurityProtocolType.Tls11 | SecurityProtocolType.Tls12;
				}

				WebClient webClient = new WebClient();

				// Get the currently installed version.
				String versionPath = Path.Combine(installLocation, "mwse-version.txt");
				String currentVersion = null;
				if (File.Exists(versionPath))
				{
					currentVersion = File.ReadAllText(versionPath).Trim();
					Console.WriteLine("Installed version: {0}", currentVersion);
				}

				// Download information from GitHub.
				var releaseInfo = DownloadSerializedJsonData<GitHubRelease>(GitHubVersionInfoURL);
				if (releaseInfo == null)
				{
					Console.WriteLine("ERROR: Could not fetch remote version information!");
#if _DEBUG
                    Console.ReadKey();
#endif
					return 2;
				}

				// Check the latest dev version.
				String latestVersion = releaseInfo.target_commitish;
				if (String.IsNullOrEmpty(latestVersion))
				{
					Console.WriteLine("ERROR: Could not determine version string!");
#if _DEBUG
                    Console.ReadKey();
#endif
					return 2;
				}
				Console.WriteLine("Newest version: {0}", latestVersion);

				// Do the versions match?
				if (currentVersion == latestVersion)
				{
					Console.WriteLine("Currently up to date.");

					// If we're supposed to start Morrowind after, do so.
					if (startAfter)
					{
						Console.WriteLine("Starting Morrowind.");
						ProcessStartInfo startInfo = new ProcessStartInfo
						{
							WorkingDirectory = installLocation,
							FileName = "Morrowind.exe"
						};
						Process.Start(startInfo);
					}

					return 0;
				}

				// Download the update.
				Console.Write("Downloading update ...");
				webClient.DownloadFile(DownloadURL, "mwse-update.zip");
				Console.WriteLine(" Done.");

				// Delete pre-restructure files.
				Console.Write("Deleting old files ...");
				DeleteFolderIfExists("Data Files\\MWSE\\lua\\mwse", true);
				DeleteFileIfExists("Data Files\\MWSE\\lua\\mwse_init.lua");
				DeleteFileIfExists("Data Files\\MWSE\\lua\\dkjson.lua");
				DeleteFileIfExists("Data Files\\MWSE\\lua\\lfs.lua");
				DeleteFileIfExists("Data Files\\MWSE\\lua\\lfs.dll");
				DeleteFileIfExists("Data Files\\Meshes\\MWSE\\Widgets.nif");
				DeleteFileIfExists("MWSE.pdb");
				DeleteFileIfExists("MWSELog.txt");
				Console.WriteLine(" Done.");

				// Delete old core files so they can be refreshed.
				if (Directory.Exists("Data Files\\MWSE\\core"))
				{
					Directory.Delete("Data Files\\MWSE\\core", true);
				}

				// Extract its contents. We can't just use ZipFile.ExtractToDirectory because it won't overwrite files.
				Console.Write("Extracting update ...");
				using (ZipArchive archive = ZipFile.Open("mwse-update.zip", ZipArchiveMode.Read))
				{
					foreach (ZipArchiveEntry file in archive.Entries)
					{
						string completeFileName = Path.Combine(installLocation, file.FullName);
						if (TemporaryInstallFiles.Contains(file.Name))
						{
							completeFileName = Path.Combine(installLocation, file.Name + ".tmp");
						}

						if (file.Name == "")
						{
							Directory.CreateDirectory(Path.GetDirectoryName(completeFileName));
							continue;
						}

						// If it's a resource file
						if (file.FullName.ToLower().Contains("data files") && !file.FullName.ToLower().Contains("data files/mwse"))
						{
							if (!overwriteResources && File.Exists(completeFileName))
								continue;
						}

						file.ExtractToFile(completeFileName, true);
					}
				}
				Console.WriteLine(" Done.");

				// Delete the update zip.
				File.Delete("mwse-update.zip");

				// Write the current version to the version cache file.
				File.WriteAllText(versionPath, latestVersion);

				// If we're supposed to start Morrowind after, do so.
				if (startAfter)
				{
					Console.WriteLine("Starting Morrowind.");
					ProcessStartInfo startInfo = new ProcessStartInfo
					{
						WorkingDirectory = installLocation,
						FileName = "Morrowind.exe"
					};
					Process.Start(startInfo);
				}

#if _DEBUG
            Console.ReadKey();
#endif
			}
			catch (Exception e)
			{
				Console.WriteLine("\nThere was a problem when updating MWSE: {0}", e.Message);
				Console.ReadKey();
			}

			return 0;
		}
	}
}
