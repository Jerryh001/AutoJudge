using IniParser;
using IniParser.Model;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Setting
{
    public partial class Setting : Form
    {
        public Setting()
        {
            InitializeComponent();
        }

        private void Setting_Load(object sender, EventArgs e)
        {
            FileIniDataParser parser = new FileIniDataParser();
            IniData ini=parser.ReadFile("config.ini");
            foreach(SectionData s in ini.Sections)
            {
                ListViewGroup g=listView1.Groups.Add(s.SectionName, s.SectionName);
                foreach(KeyData k in s.Keys)
                {
                    listView1.Items.Add(new ListViewItem(new string[] { k.KeyName, k.Value }, g));

                }
            }
            //ini.Sections
            int a=1 + 1;
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }
    }
}
