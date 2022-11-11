using System;
using System.Runtime.CompilerServices;

namespace MelonLoader.Fixes
{
	internal static class DateTimeOverride
	{
		private static TimeZoneInfo localTimeZone;

		internal static void Install()
		{
			string ltzId = GetLocalTimeZone();
			MelonDebug.Msg("Local Time Zone ID = " + ltzId);
			localTimeZone = TimeZoneInfo.FindSystemTimeZoneById(ltzId);
			Core.HarmonyInstance.Patch(typeof(TimeZoneInfo).GetProperty("Local").GetGetMethod(), typeof(DateTimeOverride).GetMethod(nameof(TimeZoneInfoLocal)).ToNewHarmonyMethod());
		}


		public static bool TimeZoneInfoLocal(ref TimeZoneInfo __result)
		{
			__result = localTimeZone;
			return false;
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern static string GetLocalTimeZone();
	}
}