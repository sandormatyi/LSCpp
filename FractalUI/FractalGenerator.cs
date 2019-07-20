using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace FractalUI
{
	public enum Direction
	{
		LEFT = 0,
		RIGHT = 1,
		UP = 2,
		DOWN = 3
	}

	public enum BlendMode
	{
		NO_ALPHA = 0,
		SMOOTH = 1,
		EPILEPSY = 2,
		SATURATED = 3
	}

	public enum TraceMode
	{
		DISABLE = 0,
		PERSIST = 1,
		FADE_FILLED = 2,
		FADE_ALL = 3
	}

	public class FractalGenerator
	{
		[DllImport("FractalGenerator.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void Init(int screenWidth, int screenHeight);

		[DllImport("FractalGenerator.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void DeInit();

		[DllImport("FractalGenerator.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void Render();

		[DllImport("FractalGenerator.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void ChangeScreenSize(int screenWidth, int screenHeight);

		[DllImport("FractalGenerator.dll", CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.BStr)]
		public static extern string GetInfoText();

		[DllImport("FractalGenerator.dll", CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.U1)]
		public static extern bool ProcessKeyboardInput(System.Windows.Forms.Keys key, bool shift);

		[DllImport("FractalGenerator.dll", CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.U1)]
		public static extern bool Step();

		[DllImport("FractalGenerator.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void SetControlParams(double zoomSpeed, double rotation, float deltaResolution);

		[DllImport("FractalGenerator.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void SetRenderParams(BlendMode blendMode, TraceMode traceMode, int enableHistogram, double fadeFactor, double alpha);

		[DllImport("FractalGenerator.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void SetPosition(double x, double y, double zoom, double rotation, float resolution);

		[DllImport("FractalGenerator.dll", CallingConvention = CallingConvention.Cdecl)]
		public static extern void ChangeColorMap(int colorMap);
	}
}
