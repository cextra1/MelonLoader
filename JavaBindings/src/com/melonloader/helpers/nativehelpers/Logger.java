package com.melonloader.helpers.nativehelpers;

import com.melonloader.ApplicationState;
import com.melonloader.Core;
import com.melonloader.LogBridge;
import com.melonloader.bridge.Assertion;

import java.io.File;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.stream.Collectors;

public class Logger
{
    static final String FilePrefix = "MelonLoader_";
    static final String FileExtension = ".log";
    static final String LatestLogFileName = "Latest";

    public static String Initialize(int MaxLogs)
    {
        File logFolderPath = Paths.get(ApplicationState.BaseDirectory, "melonloader", "etc", "logs").toFile();
        if (!logFolderPath.exists())
        {
            if (!logFolderPath.mkdir())
                Assertion.ThrowInternalFailure("Failed to Create Logs Folder!");
        }
        else
            CleanOldLogs(logFolderPath, MaxLogs);

        // Doing this here to make it easy
        File melonloaderPath = new File(logFolderPath.getParent() + "/" + LatestLogFileName + FileExtension);
        if (melonloaderPath.exists())
            melonloaderPath.delete();
        return logFolderPath.getParent();
    }

    private static void CleanOldLogs(File logFolderPath, int MaxLogs)
    {
        try {
            if (MaxLogs <= 0)
                return;

            List<File> logFiles = new ArrayList<File>();
            for (File entry : logFolderPath.listFiles())
            {
                if (!entry.isFile())
                    continue;

                String fileName = entry.getName();
                if (fileName.startsWith(FilePrefix) && fileName.endsWith(FileExtension))
                    logFiles.add(entry);
            }

            if (logFiles.size() < MaxLogs)
                return;

            logFiles = logFiles.stream().sorted(Comparator.comparingLong(File::lastModified)).collect(Collectors.toList());

            int logsToDelete = logFiles.size() - MaxLogs;
            for (int i = 0; i < logsToDelete; i++)
            {
                File log = logFiles.get(i);
                log.delete();
            }
        }
        catch (Exception e) {
            LogBridge.warning("Failed to clean log folder!");
            LogBridge.warning(e.toString());
        }
    }
}
