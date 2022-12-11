using System.Net.Sockets;

namespace MelonLoader.Fixes
{
    internal static class NetSocketsFix
    {
        internal static void Install() => Core.HarmonyInstance.Patch(typeof(SocketAsyncEventArgs).GetMethod("OnCompleted", HarmonyLib.AccessTools.all), typeof(NetSocketsFix).GetMethod(nameof(DontRunMe)).ToNewHarmonyMethod());
        public static bool DontRunMe() => false;
    }
}
