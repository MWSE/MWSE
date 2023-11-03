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
	/// The AutoUpdater class provides supporting methods for managing updates of the MWSE environment in an installation directory.
	/// </summary>
	internal static class AutoUpdater
	{
		/// <summary>
		/// Where we download the file from.
		/// </summary>
		internal static readonly string DownloadURL = "https://github.com/MWSE/MWSE/releases/download/build-automatic/mwse.zip";
		internal static readonly string GitHubVersionInfoURL = "https://api.github.com/repos/MWSE/MWSE/releases/tags/build-automatic";

		/// <summary>
		/// Files that are in use by the updater, and so need to be extracted as temporary files.
		/// These files are then swapped out when MWSE is loaded by running Morrowind.exe.
		/// </summary>
		internal static readonly HashSet<string> TemporaryInstallFiles = new HashSet<string> { "MWSE-Update.exe", "Newtonsoft.Json.dll" };

		/// <summary>
		/// Determines if Morrowind is currently running in the background.
		/// </summary>
		/// <returns>
		/// True if the process could be found, otherwise false.
		/// </returns>
		internal static bool AreGameExecutablesRunning()
		{
			return Process
				.GetProcesses()
				.Any(process => process.ProcessName.Contains("Morrowind") || process.ProcessName.Contains("TES Construction Set"));
		}

		/// <summary>
		/// Determines if the MO2 VFS module is loaded by the current process.
		/// </summary>
		/// <returns>True if the MO2 VFS is loaded.</returns>
		internal static bool IsLaunchedThroughMO2()
		{
			return Process
				.GetCurrentProcess()
				.Modules.Cast<ProcessModule>()
				.Any(module => module.ModuleName == "usvfs_x86.dll");
		}

		/// <summary>
		/// Converts a Windows directory path to a platform agnostic path.
		/// </summary>
		/// <param name="str">The windows path to convert.</param>
		/// <returns>The platform agnostic path.</returns>
		internal static string ToAgnosticPath(this string str)
		{
			return str.Replace('/', Path.PathSeparator);
		}

	}
}
