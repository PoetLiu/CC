using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Web.Script.Serialization;

namespace OneNoteSync
{
	public class _infomation {
		public string Name{ get; set; }
		public string Sex{ get; set; }
		public string Email{ get; set; }
		public string Region{ get; set; }
		public string Mobile{ get; set; }
		public string Status { get; set; }
	}
        public partial class Login : Form
        {
              //  public const string login_url = "http://myshworks.8866.org/OneNoteWWC/interface.php?login";
                public const string login_url = "http://192.168.145.131/OneNoteWWC/interface.php?Login";
                public Login()
                {
                        InitializeComponent();
                }

		private void Form1_Load(object sender, EventArgs e)
		{

		}


                private void user_TextChanged(object sender, EventArgs e)
                {

                }

                private void sure_Click(object sender, EventArgs e)
                {
                        // 认证用户
                        PostSubmitter post = new PostSubmitter();
                        post.Url = login_url;
                        post.PostItems.Add("username", user.Text);
                        post.PostItems.Add("passwd", passwd.Text);
                        post.Type = PostSubmitter.PostTypeEnum.Post;
                        string result = post.Post();
                        
			if (result.IndexOf("username or password error!")>0) {
				MessageBox.Show("密码或用户名错误！");
			} else {
				string[] str = new string[6];
				string json			= result.Replace("\"", "'");
				JavaScriptSerializer js		= new JavaScriptSerializer();
				List<_infomation> person_infomation = js.Deserialize<List<_infomation>>(json);

				for(int i=0;i<person_infomation.Count;i++){
					str[0] = person_infomation[i].Name;
					str[1] = person_infomation[i].Sex;
					str[2] = person_infomation[i].Email;
					str[3] = person_infomation[i].Region;
					str[4] = person_infomation[i].Mobile;
					str[5] = person_infomation[i].Status;
					break;
				}
				
				this.Hide();
				OneNoteSync pOneNote = new OneNoteSync(this, user.Text, passwd.Text, str);
				pOneNote.Show();
			}
                }

                private void Login_Load(object sender, EventArgs e)
                {

                }

		private void passwd_TextChanged(object sender, EventArgs e)
		{

		}

		private void checkedListBox1_SelectedIndexChanged(object sender, EventArgs e)
		{

		}
        }
}
