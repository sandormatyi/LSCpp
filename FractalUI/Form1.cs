using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace FractalUI
{
	public partial class Form1 : Form
	{
		private static volatile Mutex mut = new Mutex();
		private static volatile bool Pause = false;

		public Form1()
		{
			InitializeComponent();

			FractalGenerator.Init(Screen.PrimaryScreen.Bounds.Width, Screen.PrimaryScreen.Bounds.Height);
			RenderAndRefreshText();

			TrackbarValuesChanged(this, new EventArgs());

			Action stepAction = () =>
			{
				while (true)
				{
					if (!Pause)
					{
						mut.WaitOne();
						if (FractalGenerator.Step())
						{
							BeginInvoke((Action)delegate (){
								mut.WaitOne();
								RenderAndRefreshText();
								mut.ReleaseMutex();
							});
						}
						mut.ReleaseMutex();
					}
					Thread.Sleep(200);
				}
			};

			new Task(stepAction).Start();
		}

		private void RenderAndRefreshText()
		{
			FractalGenerator.Render();
			textBox_Log.Text = FractalGenerator.GetInfoText();
		}

		private void Form1_KeyDown(object sender, KeyEventArgs e)
		{
			mut.WaitOne();
			if (FractalGenerator.ProcessKeyboardInput(e.KeyCode, e.Shift))
			{
				RenderAndRefreshText();
			}
			mut.ReleaseMutex();

			e.Handled = true;
		}

		private void Form1_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
		{
			mut.WaitOne();
			if (FractalGenerator.ProcessKeyboardInput(e.KeyCode, e.Shift))
			{
				RenderAndRefreshText();
			}
			mut.ReleaseMutex();
		}

		private void Form1_FormClosed(object sender, FormClosedEventArgs e)
		{
			mut.WaitOne();
			FractalGenerator.DeInit();
		}

		private void TrackbarValuesChanged(object sender, EventArgs e)
		{
			mut.WaitOne();

			double zoom = Math.Pow(10, trackBar_ZoomSpeed.Value / 100.0);
			double fadeFactor = trackBar_FadeFactor.Value / 100.0;
			double alpha = trackBar_Alpha.Value / 100.0;

			FractalGenerator.SetControlParams(zoom, trackBar_RotationSpeed.Value, trackBar_DeltaResolution.Value);
			FractalGenerator.ChangeColorMap(
				comboBox_ColorMap.SelectedIndex < 0 ? 0 : comboBox_ColorMap.SelectedIndex);

			FractalGenerator.SetRenderParams(
				(BlendMode)(comboBox_BlendMode.SelectedIndex < 0 ? 0 : comboBox_BlendMode.SelectedIndex),
				(TraceMode)(comboBox_TraceMode.SelectedIndex < 0 ? 0 : comboBox_TraceMode.SelectedIndex),
				checkBox_EnableHistogra.Checked ? 1 : 0,
				fadeFactor, alpha);

			RenderAndRefreshText();

			mut.ReleaseMutex();

			textBox_ZoomSpeed.Text = zoom.ToString();
			textBox_Rotation.Text = trackBar_RotationSpeed.Value.ToString();
			textBox_Resolution.Text = trackBar_DeltaResolution.Value.ToString();
			textBox_FadeFactor.Text = fadeFactor.ToString();
			textBox_Alpha.Text = alpha.ToString();
		}

		private void button_Pause_Click(object sender, EventArgs e)
		{
			Pause = !Pause;
			if (Pause)
			{
				button_Pause.Text = "Start";
			}
			else
			{
				button_Pause.Text = "Pause";
			}
		}

		private void button_Reset_Click(object sender, EventArgs e)
		{
			mut.WaitOne();
			FractalGenerator.SetPosition(0, 0, 1, 0, 1024);
			RenderAndRefreshText();
			mut.ReleaseMutex();
		}
	}
}
