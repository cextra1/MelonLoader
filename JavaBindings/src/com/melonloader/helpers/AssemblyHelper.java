package com.melonloader.helpers;

import android.content.res.AssetManager;

import android.util.Log;
import com.melonloader.ApplicationState;
import com.melonloader.LogBridge;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.Paths;
import java.util.Objects;

public class AssemblyHelper {
    private static AssemblyHelper instance;

    AssemblyHelper()
    {
        instance = this;
    }

    public static boolean InstallAssemblies()
    {
        if (!ContextHelper.CheckContext("Cannot install assemblies"))
            return false;

        LogBridge.msg("Installing Assemblies");

        new AssemblyHelper();

        //AssetManager am = ApplicationState.Context.getAssets();

        return AssemblyHelper.instance.CopyAssetsTo(new File(ApplicationState.BaseDirectory));
    }

    private boolean CopyAssetsTo(File path)
    {
        AssetManager am = ApplicationState.Context.getAssets();

        String[] files;

        try {
            files = am.list("");
        } catch (IOException e) {
            LogBridge.error(e.getMessage());
            return false;
        }

        File etcFolder = Paths.get(path.toString(), "il2cpp", "etc").toFile();
        if (!etcFolder.exists()) {
            LogBridge.msg("Installing il2cpp/etc");

            etcFolder.mkdir();
            File monoFolder = new File(etcFolder, "mono");
            monoFolder.mkdir();

            /* auto-generated */

            new File(monoFolder, "2.0").mkdir();
            new File(monoFolder, "2.0/Browsers").mkdir();
            new File(monoFolder, "4.0").mkdir();
            new File(monoFolder, "4.0/Browsers").mkdir();
            new File(monoFolder, "4.5").mkdir();
            new File(monoFolder, "4.5/Browsers").mkdir();
            new File(monoFolder, "mconfig").mkdir();

            copyFile("bin/Data/Managed/etc/mono/2.0/Browsers/Compat.browser", new File(monoFolder, "2.0/Browsers/Compat.browser"), false);
            copyFile("bin/Data/Managed/etc/mono/2.0/DefaultWsdlHelpGenerator.aspx", new File(monoFolder, "2.0/DefaultWsdlHelpGenerator.aspx"), false);
            copyFile("bin/Data/Managed/etc/mono/2.0/machine.config", new File(monoFolder, "2.0/machine.config"), false);
            copyFile("bin/Data/Managed/etc/mono/2.0/settings.map", new File(monoFolder, "2.0/settings.map"), false);
            copyFile("bin/Data/Managed/etc/mono/2.0/web.config", new File(monoFolder, "2.0/web.config"), false);
            copyFile("bin/Data/Managed/etc/mono/4.0/Browsers/Compat.browser", new File(monoFolder, "4.0/Browsers/Compat.browser"), false);
            copyFile("bin/Data/Managed/etc/mono/4.0/DefaultWsdlHelpGenerator.aspx", new File(monoFolder, "4.0/DefaultWsdlHelpGenerator.aspx"), false);
            copyFile("bin/Data/Managed/etc/mono/4.0/machine.config", new File(monoFolder, "4.0/machine.config"), false);
            copyFile("bin/Data/Managed/etc/mono/4.0/settings.map", new File(monoFolder, "4.0/settings.map"), false);
            copyFile("bin/Data/Managed/etc/mono/4.0/web.config", new File(monoFolder, "4.0/web.config"), false);
            copyFile("bin/Data/Managed/etc/mono/4.5/Browsers/Compat.browser", new File(monoFolder, "4.5/Browsers/Compat.browser"), false);
            copyFile("bin/Data/Managed/etc/mono/4.5/DefaultWsdlHelpGenerator.aspx", new File(monoFolder, "4.5/DefaultWsdlHelpGenerator.aspx"), false);
            copyFile("bin/Data/Managed/etc/mono/4.5/machine.config", new File(monoFolder, "4.5/machine.config"), false);
            copyFile("bin/Data/Managed/etc/mono/4.5/settings.map", new File(monoFolder, "4.5/settings.map"), false);
            copyFile("bin/Data/Managed/etc/mono/4.5/web.config", new File(monoFolder, "4.5/web.config"), false);
            copyFile("bin/Data/Managed/etc/mono/mconfig/config.xml", new File(monoFolder, "mconfig/config.xml"), false);
            copyFile("bin/Data/Managed/etc/mono/browscap.ini", new File(monoFolder, "browscap.ini"), false);
            copyFile("bin/Data/Managed/etc/mono/config", new File(monoFolder, "config"), false);

            /* end auto-generated */

            //LogBridge.msg("----------------------------------------");
        }

        this.copyFileOrDir("melonloader", path);

        return true;
    }

    public void copyFileOrDir(String path, File base) {
        AssetManager assetManager = ApplicationState.Context.getAssets();
        String assets[] = null;
        try {
            assets = assetManager.list(path);
            if (assets.length == 0) {
                copyFile(path, base);
            } else {
                String fullPath = base + "/" + path;
                File dir = new File(fullPath);
                if (!dir.exists())
                    dir.mkdir();
                for (int i = 0; i < assets.length; ++i) {
                    //LogBridge.msg("Installing " + assets[i]);
                    copyFileOrDir(path + "/" + assets[i], base);
                }
            }
        } catch (IOException ex) {
            LogBridge.error(ex.getMessage());
        }
    }

    private void copyFile(String filename, File base) {
        AssetManager assetManager = ApplicationState.Context.getAssets();

        InputStream in = null;
        OutputStream out = null;
        try {
            in = assetManager.open(filename);
            String newFileName = base + "/" + filename;
            out = new FileOutputStream(newFileName);

            byte[] buffer = new byte[1024];
            int read;
            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
            in.close();
            in = null;
            out.flush();
            out.close();
            out = null;
        } catch (Exception e) {
            LogBridge.error(e.getMessage());
        }
    }

    private void copyFile(String filename, File base, boolean differentiatorBecauseImTiredAndCantThinkOfABetterSolution) {
        AssetManager assetManager = ApplicationState.Context.getAssets();

        InputStream in = null;
        OutputStream out = null;
        try {
            in = assetManager.open(filename);
            String newFileName = base.toString();
            out = new FileOutputStream(newFileName);

            byte[] buffer = new byte[1024];
            int read;
            while ((read = in.read(buffer)) != -1) {
                out.write(buffer, 0, read);
            }
            in.close();
            in = null;
            out.flush();
            out.close();
            out = null;
        } catch (Exception e) {
            LogBridge.error(e.getMessage());
        }
    }
}
