using System;

namespace MelonLoader.Fixes
{
	internal static class DateTimeOverride
	{
		internal static void Install()
		{
			Core.HarmonyInstance.Patch(typeof(DateTime).GetProperty("Now").GetGetMethod(), typeof(ForcedCultureInfo).GetMethod("DateTimeNow").ToNewHarmonyMethod());
		}

		public static bool DateTimeNow(ref DateTime __result)
		{
			__result = default;
			return false;
		}
	}
}