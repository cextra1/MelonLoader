using MelonLoader.Modules;
using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.CompilerServices;

namespace MelonLoader.InternalUtils
{
    internal static class Il2CppAssemblyGenerator
    {
#if __ANDROID__
        public static readonly MelonModule.Info moduleInfo = new MelonModule.Info(
            Path.Combine(MelonUtils.GetApplicationPath(), $"melonloader/etc/assembly_generation/managed/Il2CppAssemblyGenerator.dll")
            , () => !MelonUtils.IsGameIl2Cpp());
#else
        public static readonly MelonModule.Info moduleInfo = new MelonModule.Info(
            $"MelonLoader{Path.DirectorySeparatorChar}Dependencies{Path.DirectorySeparatorChar}Il2CppAssemblyGenerator{Path.DirectorySeparatorChar}Il2CppAssemblyGenerator.dll"
            , () => !MelonUtils.IsGameIl2Cpp());
#endif

        internal static bool Run()
        {
            var module = MelonModule.Load(moduleInfo);
            if (module == null)
                return true;

            MelonLogger.Msg("Loading Il2CppAssemblyGenerator...");

            MonoInternals.MonoResolveManager.GetAssemblyResolveInfo("Il2CppAssemblyGenerator").Override = module.Assembly;

#if !__ANDROID__
            IntPtr windowHandle = Process.GetCurrentProcess().MainWindowHandle;
            DisableCloseButton(windowHandle);
            var ret = module.SendMessage("Run");
            EnableCloseButton(windowHandle);
#else
            var ret = module.SendMessage("Run");
#endif
            MelonUtils.SetCurrentDomainBaseDirectory(MelonUtils.GameDirectory);
            return ret is int retVal && retVal == 0;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static void EnableCloseButton(IntPtr mainWindow);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static void DisableCloseButton(IntPtr mainWindow);
    }
}