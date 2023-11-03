/*
/// The main auto updater program. The program will do the following:
/// 1) Reads the GitHub API to find the commit hash for the latest release.
/// 2) Compares its information to the local mwse-version.txt file.
/// 3) If the version doesn't match, it downloads the file from https://github.com/MWSE/MWSE/releases/download/build-automatic/mwse.zip
/// 4) Extracts the contents and cleans up after itself.
/// 5) Saves the new version as mwse-version.txt.
*/

using System.Diagnostics;
using System.IO.Compression;
using System.Net;
using System.Net.Http.Headers;
using System.Text.Json;
using Microsoft.Win32;
using MWSE;

// Prevent updating through MO2.
if (AutoUpdater.IsLaunchedThroughMO2())
{
	Console.WriteLine("Updating MWSE through MO2 can lead to duplicate core files, out of date files, and has caused support headaches in the past. It is no longer supported.");
	Console.WriteLine("Press any key to exit...");
	Console.ReadKey();
	return 1;
}

// Check to see if Morrowind is running; wait for it to close.
if (AutoUpdater.AreGameExecutablesRunning())
{
	Console.WriteLine("Waiting for Morrowind and the Construction Set to close...");
	while (AutoUpdater.AreGameExecutablesRunning())
	{
		Thread.Sleep(1000);
	}
}

// Check to see if we want to start the game after checking for updates.
var startAfter = args.Contains("-startAfter");

// Check to see if we want to specifically overwrite existing non-script files.
var overwriteResources = args.Contains("-overwriteResources");

// Try to find Morrowind's install location, store it here.
string? installLocation = null;

// First we'll check the current directory.
if (File.Exists("Morrowind.exe"))
{
	installLocation = AppDomain.CurrentDomain.BaseDirectory;
}
// Fall back to the registry.
else if (string.IsNullOrEmpty(installLocation) && OperatingSystem.IsWindows())
{
	var registryValue = Registry.GetValue("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\bethesda softworks\\Morrowind", "Installed Path", null) as string;
	if (!string.IsNullOrEmpty(registryValue) && File.Exists(Path.Combine(registryValue, "Morrowind.exe")))
	{
		installLocation = registryValue;
	}
}

// Make sure that we actually have a valid install location.
if (!string.IsNullOrEmpty(installLocation))
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
	// On Windows versions before 8.1, we have to force TLS support. We also force it for non-Windows OS.
	if (!OperatingSystem.IsWindows() || Environment.OSVersion.Platform == PlatformID.Win32NT && Environment.OSVersion.Version.CompareTo(new Version(6, 3)) < 0)
	{
		ServicePointManager.SecurityProtocol = SecurityProtocolType.Tls | SecurityProtocolType.Tls11 | SecurityProtocolType.Tls12;
	}

	var client = new HttpClient(new HttpClientHandler()
	{
		PreAuthenticate = true,
		UseProxy = false,
	});
	client.DefaultRequestHeaders.Add("User-Agent", "MWSE Updater");
	client.DefaultRequestHeaders.Pragma.Clear();

	// Get the currently installed version.
	var versionPath = Path.Combine(installLocation, "mwse-version.txt");
	string? currentVersion = null;
	if (File.Exists(versionPath))
	{
		currentVersion = File.ReadAllText(versionPath).Trim();
		Console.WriteLine("Installed version: {0}", currentVersion);
	}

	// Download information from GitHub.
	var result = string.Empty;
	try
	{
		result = await client.GetStringAsync(AutoUpdater.GitHubVersionInfoURL);
	}
	catch (Exception e)
	{
		Console.WriteLine("ERROR: API call to GitHub Version Info URL failed!");
		Console.WriteLine(e);
		throw;
	}
	
	var releaseInfo = JsonSerializer.Deserialize<GitHubRelease>(result);
	
	if (releaseInfo == null)
	{
		Console.WriteLine("ERROR: Could not fetch remote version information!");
#if _DEBUG
        Console.ReadKey();
#endif
		return 2;
	}

	// Check the latest dev version.
	var latestVersion = releaseInfo.target_commitish;
	if (string.IsNullOrEmpty(latestVersion))
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
			var startInfo = new ProcessStartInfo
			{
				WorkingDirectory = installLocation,
				FileName = "Morrowind.exe"
			};
			Process.Start(startInfo);
		}

		return 0;
	}

	// Download the update.
	Console.WriteLine("Downloading update ...");
	var message = await client.GetAsync(AutoUpdater.DownloadURL);
	
	await using (var stream = message.Content.ReadAsStream())
	{
		Console.WriteLine("Found update of {0} size.", stream.Length);

		await using (var file = File.Create("mwse-update.zip"))
		{
			stream.Seek(0, SeekOrigin.Begin);
			await stream.CopyToAsync(file);
		}
	}
	
	Console.WriteLine("Done.");

	// Delete pre-restructure files.
	Console.Write("Deleting old files ...");
	if (Directory.Exists("Data Files\\MWSE\\lua\\mwse".ToAgnosticPath()))
	{
		Directory.Delete("Data Files\\MWSE\\lua\\mwse".ToAgnosticPath(), true);
	}
	if (File.Exists("Data Files\\MWSE\\lua\\mwse_init.lua".ToAgnosticPath()))
	{
		File.Delete("Data Files\\MWSE\\lua\\mwse_init.lua".ToAgnosticPath());
	}
	if (File.Exists("Data Files\\MWSE\\lua\\dkjson.lua".ToAgnosticPath()))
	{
		File.Delete("Data Files\\MWSE\\lua\\dkjson.lua".ToAgnosticPath());
	}
	if (File.Exists("Data Files\\MWSE\\lua\\lfs.lua".ToAgnosticPath()))
	{
		File.Delete("Data Files\\MWSE\\lua\\lfs.lua".ToAgnosticPath());
	}
	if (File.Exists("Data Files\\MWSE\\lua\\lfs.dll".ToAgnosticPath()))
	{
		File.Delete("Data Files\\MWSE\\lua\\lfs.dll".ToAgnosticPath());
	}
	if (File.Exists("MWSE.pdb"))
	{
		File.Delete("MWSE.pdb");
	}
	if (File.Exists("MWSELog.txt"))
	{
		File.Delete("MWSELog.txt");
	}
	Console.WriteLine(" Done.");

	// Delete old core files so they can be refreshed.
	if (Directory.Exists("Data Files\\MWSE\\core".ToAgnosticPath()))
	{
		Directory.Delete("Data Files\\MWSE\\core".ToAgnosticPath(), true);
	}

	// Extract its contents. We can't just use ZipFile.ExtractToDirectory because it won't overwrite files.
	Console.Write("Extracting update ...");
	using (var archive = ZipFile.Open("mwse-update.zip", ZipArchiveMode.Read))
	{
		foreach (var file in archive.Entries)
		{
			var completeFileName = Path.Combine(installLocation, file.FullName);
			if (AutoUpdater.TemporaryInstallFiles.Contains(file.Name))
			{
				completeFileName = Path.Combine(installLocation, file.Name + ".tmp");
			}

			if (file.Name == "")
			{
				var directory = Path.GetDirectoryName(completeFileName);

				if (directory == null)
					throw new DirectoryNotFoundException($"Unable to find directory for ${completeFileName}");
				
				Directory.CreateDirectory(directory);
				continue;
			}

			// If it's a resource file
			if (file.FullName.ToLower().Contains("data files") && !file.FullName.ToLower().Contains("data files/mwse".ToAgnosticPath()))
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
		var startInfo = new ProcessStartInfo
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
