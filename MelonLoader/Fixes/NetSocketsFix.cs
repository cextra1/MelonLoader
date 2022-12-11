using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;

namespace MelonLoader.Fixes
{
    internal static class NetSocketsFix
    {
        internal static void Install() => Core.HarmonyInstance.Patch(typeof(SocketAsyncEventArgs).GetProperty("OnCompleted").GetGetMethod(), typeof(NetSocketsFix).GetMethod(nameof(DontRunMe)).ToNewHarmonyMethod());
        public static bool DontRunMe() => false;
    }
}
