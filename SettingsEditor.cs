using System;
using System.Diagnostics;
using System.IO;
using System.Text.Json;
using System.Windows.Forms;
using System.Collections.Generic;

namespace YgoMasterSettingsEditor
{
    public partial class MainForm : Form
    {
        private string settingsPath;
        private Dictionary<string, object> settings;
        private Dictionary<string, TextBox> entryBoxes = new Dictionary<string, TextBox>();

        public MainForm()
        {
            InitializeComponent();
            Button loadButton = new Button() { Text = "Load settings.json", Dock = DockStyle.Top };
            loadButton.Click += LoadButton_Click;
            Controls.Add(loadButton);
        }

        private void LoadButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.Filter = "JSON files|settings.json";
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                settingsPath = ofd.FileName;
                string json = File.ReadAllText(settingsPath);
                settings = JsonSerializer.Deserialize<Dictionary<string, object>>(json);
                RenderFields();
            }
        }

        private void RenderFields()
        {
            Controls.Clear();
            int y = 10;
            entryBoxes.Clear();
            foreach (var kv in settings)
            {
                Label lbl = new Label() { Text = kv.Key, Left = 10, Top = y, Width = 120 };
                TextBox txt = new TextBox() { Left = 140, Top = y, Width = 200, Text = kv.Value?.ToString() ?? "" };
                Controls.Add(lbl);
                Controls.Add(txt);
                entryBoxes[kv.Key] = txt;
                y += 30;
            }

            Button saveButton = new Button() { Text = "Save", Left = 10, Top = y, Width = 80 };
            saveButton.Click += SaveButton_Click;
            Controls.Add(saveButton);

            Button startButton = new Button() { Text = "Start YgoMaster", Left = 100, Top = y, Width = 120 };
            startButton.Click += StartButton_Click;
            Controls.Add(startButton);
        }

        private void SaveButton_Click(object sender, EventArgs e)
        {
            foreach (var kv in entryBoxes)
                settings[kv.Key] = kv.Value.Text;
            string json = JsonSerializer.Serialize(settings, new JsonSerializerOptions { WriteIndented = true });
            File.WriteAllText(settingsPath, json);
            MessageBox.Show("Saved!");
        }

        private void StartButton_Click(object sender, EventArgs e)
        {
            string exePath = Path.Combine(Path.GetDirectoryName(settingsPath), "..", "ygomasterclient.exe");
            exePath = Path.GetFullPath(exePath);
            if (!File.Exists(exePath))
            {
                MessageBox.Show("ygomasterclient.exe not found: " + exePath);
                return;
            }
            Process.Start(exePath);
        }
    }
}