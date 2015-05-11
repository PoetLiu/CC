using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.Net.Security;
using System.Security.Cryptography.X509Certificates;
using System.Net;
using System.IO;
using System.IO.Compression;
using System.Text.RegularExpressions;
using System.Web.Script.Serialization;
using System.Runtime.Serialization.Json;
using System.Runtime.Serialization.Formatters.Binary;
using System.Xml.Linq;
using Microsoft.Office.Interop.OneNote;
using System.Collections;
using System.Threading; 

namespace OneNoteSync
{
	[Serializable]
	public class project
	{
		public string Code { get; set; }			//要和JSON数据中的字符匹配，否则取出来是为空的
	}

	public class Task
	{
		public string ID { get; set; }			//要和JSON数据中的字符匹配，否则取出来是为空的
		public string Name { get; set; }
		public string Status { get; set; }
	}

        public class server_info
        {
                public string host { get; set; }
                public string port { get; set; }
                public string number { get; set; }

        }

        public partial class OneNoteSync : Form
        {

		System.Windows.Forms.Panel bigPanel = new Panel();
		private System.Windows.Forms.TextBox tb1;
		private System.Windows.Forms.CheckBox choose;
		private System.Windows.Forms.Button add;
		private System.Windows.Forms.CheckedListBox checkedListBox1;

		List<string> s = new List<string>();
		List<string> s1 = new List<string>();
		List<int> recorder = new List<int>();
		List<string> project_name=new List<string>();
		int record = 0;
		int flag = 0;
		int num = 0;
		PostSubmitter post = null;

		List<Task>[] tasks = null;			// 任务列表
		List<project> project_code = null;		// 项目代号列表

		JavaScriptSerializer js=null;
                
                public const string get_project_url = "http://192.168.145.131/OneNoteWWC/interface.php?get_project";
                public const string get_task_url = "http://192.168.145.131/OneNoteWWC/interface.php?get_task";
                public const string add_projectcontent_url = "http://192.168.145.131/OneNoteWWC/interface.php?add_projectcontent";
                public const string get_projectcontent_url = "http://192.168.145.131/OneNoteWWC/interface.php?get_projectcontent";
                public OneNoteSync()
                {
                        InitializeComponent();
                }

		private void structs()
		{
		// 本地自动生成笔记本结构
			// 创建一个新的OneNote控制台
			var onenoteApp = new Microsoft.Office.Interop.OneNote.Application();
			// 打开OneNote，并且XML的方式操作
			string notebookXml;
			onenoteApp.GetHierarchy(null, HierarchyScope.hsPages, out notebookXml);
			var onenote_doc = XDocument.Parse(notebookXml);
			var onenote_namespace = onenote_doc.Root.Name.Namespace;
			
			List<string> notebook_name = new List<string>();
			String notebook_ID, section_ID, page_ID;
			bool exists=false;
			foreach (var notebook in from node in onenote_doc.Descendants(onenote_namespace + "Notebook") select node) {
				if (notebook.Attribute("name").Value.Equals("Work")) {
					exists = true;
					break;
				}
			}
			if (exists==false) {
                                onenoteApp.OpenHierarchy("E:\\temp\\Work", System.String.Empty, out notebook_ID, CreateFileType.cftNotebook);
			}

			for (int i = 0; i < project_code.Count(); i++) {
                                onenoteApp.OpenHierarchy("E:\\temp\\Work\\" + project_code[i].Code + ".one", System.String.Empty, out section_ID, CreateFileType.cftSection);
				for (int j = 0; j < tasks[i].Count(); j++) {
					onenoteApp.CreateNewPage(section_ID, out page_ID, NewPageStyle.npsBlankPageNoTitle);
					var page = new XDocument(new XElement(onenote_namespace + "Page",
								   new XElement(onenote_namespace + "Title",
									   new XElement(onenote_namespace + "OE",
										   new XElement(onenote_namespace + "T",
											   new XCData(tasks[i][j].Name))))));
					page.Root.SetAttributeValue("ID", page_ID);
					onenoteApp.UpdatePageContent(page.ToString(), DateTime.MinValue);
				}
			}

		}

		private void Form1_Load(object sender, EventArgs e)
		{
			
			string json;
                        int fa_num;
			List<Task> task;

			post = new PostSubmitter();
			post.Url = get_project_url;
			post.PostItems.Add("username", username);
			post.PostItems.Add("passwd", password);
			post.Type = PostSubmitter.PostTypeEnum.Post;

			string result;
			
			try {
				result = post.Post();
				json = result.Replace("\"", "'");
				js = new JavaScriptSerializer();
				project_code = js.Deserialize<List<project>>(json);			//将 JSON 格式字符串转换为指定类型的对象。
			}catch {
				MessageBox.Show("URL 异常");
			}

			fa_num = project_code.Count;                                                       //计算有多少个项目
			post.Url = get_task_url;
			tasks = new List<Task>[fa_num];

			for(int i=0;i<fa_num;i++){
				post.PostItems.Clear();
				post.PostItems.Add("username", username);
				post.PostItems.Add("passwd", password);
				post.PostItems.Add("project_code", project_code[i].Code);
				try {
					result = post.Post();
					json = result.Replace("\"", "'");
					task = js.Deserialize<List<Task>>(json);
					tasks[i] = task;
				}catch{
					MessageBox.Show("URL异常");
					break;
				}
			}

			// 自动化建立笔记本结构
//			structs();

			for (int i = 0; i < fa_num; i++){
				this.choose = new System.Windows.Forms.CheckBox();
				this.choose.AutoSize = true;
				this.choose.Location = new System.Drawing.Point(50, 80 + i * 23);
				this.choose.Name = i.ToString();
				this.choose.Size = new System.Drawing.Size(70, 22);
				this.choose.TabIndex = 0;
				this.choose.UseVisualStyleBackColor = true;
				this.choose.CheckedChanged += new System.EventHandler(this.choose_CheckedChanged);
				bigPanel.Controls.Add(this.choose);

				this.tb1 = new System.Windows.Forms.TextBox();
				this.tb1.Location = new System.Drawing.Point(85, choose.Bottom - choose.Height);
				this.tb1.Name = i.ToString();
				this.tb1.Size = new System.Drawing.Size(75, 30);
				this.tb1.BackColor = Color.Gray;
				this.tb1.TabIndex = 4;
				//this.tb1.AutoScrollOffset(10, 0);
				//this.tb1.Size = new Size(50, 20);
				this.tb1.Text = project_code[i].Code;
				//只读模式
				this.tb1.ReadOnly = true;
				bigPanel.Controls.Add(this.tb1);
				//this.Controls.Add(this.choose);

				this.add = new System.Windows.Forms.Button();
				this.add.AutoSize = true;
				this.add.Location = new System.Drawing.Point(165, choose.Bottom - choose.Height);
				this.add.Name = i.ToString();
				this.add.Size = new System.Drawing.Size(30, 22);
				this.add.TabIndex = 0;
				this.add.FlatAppearance.BorderSize = 0;
				this.add.UseVisualStyleBackColor = true;
				//点击消息函数
				this.add.Click += new System.EventHandler(this.button2_Click_1);
				bigPanel.Controls.Add(this.add);
				//this.Controls.Add(this.choose);

				this.checkedListBox1 = new System.Windows.Forms.CheckedListBox();
				this.checkedListBox1.FormattingEnabled = true;
				this.checkedListBox1.Location = new System.Drawing.Point(choose.Right + 20, tb1.Bottom);
				this.checkedListBox1.Name = i.ToString();
				this.checkedListBox1.Size = new System.Drawing.Size(120, 96);
				this.checkedListBox1.BackColor = Color.Gray;
				this.checkedListBox1.Visible = false;
				this.checkedListBox1.HorizontalScrollbar = true;
				//CheckedListBox表中判断点击消息函数
				checkedListBox1.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.checkedListBox1_ItemCheck);
				bigPanel.Controls.Add(this.checkedListBox1);
				//this.Controls.Add(this.choose);
			}

			Button Mybutton = new Button();
			// 			Mybutton.MouseEnter += new System.EventHandler(this.btn_MouseEnter);
			// 			Mybutton.MouseLeave += new System.EventHandler(this.btn_MouseLeave);
			Mybutton.Click += new System.EventHandler(this.button1_Click);
			Mybutton.Name = "button";
			Mybutton.Text = "下载";
			Mybutton.Location = new Point(300, 100);
			Mybutton.Size = new Size(50, 28);
			Mybutton.ForeColor = System.Drawing.Color.Blue;
			Mybutton.BackColor = Color.Gray;
			Mybutton.FlatStyle = FlatStyle.Flat;
			Mybutton.FlatAppearance.BorderSize = 0;
			Mybutton.Font = new Font(Mybutton.Font.FontFamily, 10, Mybutton.Font.Style);
			this.Controls.Add(Mybutton);

			Button Mybutton1 = new Button();
			Mybutton1.Name = "button";
			Mybutton1.Text = "同步";
			Mybutton1.Location = new Point(300, 150);
			Mybutton1.Size = new Size(50, 28);
			Mybutton1.ForeColor = System.Drawing.Color.Blue;
			Mybutton1.BackColor = Color.Gray;
			Mybutton1.FlatStyle = FlatStyle.Flat;
			Mybutton1.FlatAppearance.BorderSize = 0;
			//Mybutton.Font.Size = 20;
			Mybutton.Font = new Font(Mybutton1.Font.FontFamily, 10, Mybutton1.Font.Style);
			this.Controls.Add(Mybutton1);

			Button Mybutton2 = new Button();
			//Mybutton1.Click += new System.EventHandler(this.button1_Click);
			Mybutton2.Name = "button";
			Mybutton2.Text = "切换用户";
			Mybutton2.Location = new Point(420, 200);
			Mybutton2.Size = new Size(80, 28);
			Mybutton2.ForeColor = System.Drawing.Color.Blue;
			Mybutton2.BackColor = Color.Gray;
			Mybutton2.FlatStyle = FlatStyle.Flat;
			Mybutton2.FlatAppearance.BorderSize = 0;
			//Mybutton.Font.Size = 20;
			Mybutton2.Font = new Font(Mybutton2.Font.FontFamily, 10, Mybutton2.Font.Style);
			this.Controls.Add(Mybutton2);

			TextBox tbox0 = new TextBox();
			tbox0.Text = str1[0];
			tbox0.ReadOnly = true;
			tbox0.BackColor = Color.CornflowerBlue;
			tbox0.Location = new Point(300, 180);
			this.Controls.Add(tbox0);

			TextBox tbox = new TextBox();
			tbox.Text = str1[1];
			tbox.ReadOnly = true;
			tbox.BackColor = Color.CornflowerBlue;
			tbox.Location = new Point(300, 200);
			this.Controls.Add(tbox);

			TextBox tbox1 = new TextBox();
			tbox1.Text = str1[3];
			tbox1.ReadOnly = true;
			tbox1.BackColor = Color.CornflowerBlue;
			tbox1.Location = new Point(300, 220);
			this.Controls.Add(tbox1);

			TextBox tbox2 = new TextBox();
			tbox2.Text = str1[4];
			tbox2.ReadOnly = true;
			tbox2.BackColor = Color.CornflowerBlue;
			tbox2.Location = new Point(300, 240);
			this.Controls.Add(tbox2);

			TextBox tbox3 = new TextBox();
			tbox3.Text = str1[2];
			tbox3.ReadOnly = true;
			tbox3.BackColor = Color.CornflowerBlue;
			tbox3.Location = new Point(300, 260);
			this.Controls.Add(tbox3);

			TextBox tbox4 = new TextBox();
			tbox4.Text = str1[5];
			tbox4.ReadOnly = true;
			tbox4.BackColor = Color.CornflowerBlue;
			tbox4.Location = new Point(300, 280);
			this.Controls.Add(tbox4);

			bigPanel.Location = new Point(30, 80);
			bigPanel.Size = new Size(220, 200);
			bigPanel.AutoScroll = true;
			Controls.Add(bigPanel);
		
		}

                private void isAbort_CheckedChanged(object sender, EventArgs e)
                {
			int i;
			int j = 0;      //加载任务
			int m = -1;
			int chancel = -1;
			CheckBox currentButton = (CheckBox)sender;
			List<string> ls;
			ls = s1;
			CheckedListBox tanfser;
			int index = 0;
			string[] kk = ls.ToArray();

			if (currentButton.Checked)
			{
				foreach (Control aControl in bigPanel.Controls)
				{
					if (aControl is CheckedListBox)
					{
						tanfser = (CheckedListBox)aControl;
						if (index == 0)
						{
							int son_num = tasks[j].Count;
							for (i = 0; i < son_num; i++)
							{
								m = tasks[j][i].Status.IndexOf("中止");
								if (m != -1)
								{
									tanfser.Items.Add(tasks[j][i].ID + " " + tasks[j][i].Name + " " + tasks[j][i].Status);
								}

							}
							j++;
						}

					}
				}
			}
			else
			{
				foreach (Control aControl in bigPanel.Controls)
				{
					if (aControl is CheckedListBox)
					{
						tanfser = (CheckedListBox)aControl;
						for (i = 0; i < tanfser.Items.Count; i++)
						{
							if (tanfser.GetItemChecked(i) == false)
							{
								//判断CheckedListBox某行中是否存在某个特定的字符串
								chancel = tanfser.Items[i].ToString().IndexOf("中止");
								if (chancel != -1)
								{
									tanfser.Items.RemoveAt(i);
									i--;
								}
							}
						}
					}
				}
			}

                }

                private void download_Click(object sender, EventArgs e)
                {
			if (post == null)
				post = new PostSubmitter();
			post.Url = get_projectcontent_url;
			post.PostItems.Clear();
			post.PostItems.Add("username", username);
			post.PostItems.Add("passwd", password);
			post.PostItems.Add("project_code", "Exon");
			post.Type = PostSubmitter.PostTypeEnum.Post;
			string OEChildrenNode = post.Post();

			int length = OEChildrenNode.Length;
			// 创建一个新的OneNote控制台
			var onenoteApp = new Microsoft.Office.Interop.OneNote.Application();

			// 打开OneNote，并且XML的方式操作
			string notebookXml;
			onenoteApp.GetHierarchy(null, HierarchyScope.hsPages, out notebookXml);
			var onenote_doc = XDocument.Parse(notebookXml);			// 获取一个“onenote文件”，即一个使用包含XML的字符串填充的System.Xml.Linq.XDocument
			var onenote_namespace = onenote_doc.Root.Name.Namespace;	// 获取一个“onenote命名空间”，即一个System.Xml.Linq.XNamespace，其中包含名称的命名空间部分
			// 获取指定的page元素，获得此元素ID
			var pageNode = onenote_doc.Descendants(onenote_namespace + "Page").Where(n => n.Attribute("name").Value == "MFC Putty界面部分实现（袁会）").FirstOrDefault();
			var PageNodeID = pageNode.Attribute("ID").Value;

			if (pageNode != null) {
				// 将获取的page页面以XML的方式操作，获得outline元素及其objectID
				string pageXml;
				onenoteApp.GetPageContent(PageNodeID, out pageXml);
				var page_doc = XDocument.Parse(pageXml);
				var page_namespace = page_doc.Root.Name.Namespace;
				XElement outlineNode = page_doc.Descendants(page_namespace + "Outline").FirstOrDefault();
				var OutlineNodeID = outlineNode.Attribute("objectID").Value;
				// 删除已存在的outline内容，
				try {
					onenoteApp.DeletePageContent(PageNodeID, OutlineNodeID);
				} catch {
				}
			}
				// 插入新内容
				var page = new XDocument(new XElement(onenote_namespace + "Page",
							   new XElement(onenote_namespace + "Outline",
							     new XElement(OEChildrenNode))));
				page.Root.SetAttributeValue("ID", PageNodeID);
				onenoteApp.UpdatePageContent(page.ToString(), DateTime.MinValue);
                }

                private void sync_Click(object sender, EventArgs e)
                {
			// 创建一个新的OneNote控制台
			var onenoteApp = new Microsoft.Office.Interop.OneNote.Application();
			
			// 打开OneNote，并且XML的方式操作
			string notebookXml;
			onenoteApp.GetHierarchy(null, HierarchyScope.hsPages, out notebookXml);
			var onenote_doc = XDocument.Parse(notebookXml);			// 获取一个“onenote文件”，即一个使用包含XML的字符串填充的System.Xml.Linq.XDocument
			var onenote_namespace = onenote_doc.Root.Name.Namespace;	// 获取一个“onenote命名空间”，即一个System.Xml.Linq.XNamespace，其中包含名称的命名空间部分
			
			// 获取指定的page元素，获得此元素ID
			var pageNode = onenote_doc.Descendants(onenote_namespace + "Page").Where(n => n.Attribute("name").Value == "OneNote XML格式").FirstOrDefault();
			var PageNodeID = pageNode.Attribute("ID").Value;
                        string result;
                        if (pageNode != null)
                        {

                                // 将获取的page页面以XML的方式操作，获得outline元素及其objectID
                                string pageXml;

                                onenoteApp.GetPageContent(PageNodeID, out pageXml);
                                var page_doc = XDocument.Parse(pageXml);
                                var page_namespace = page_doc.Root.Name.Namespace;
                                XElement outlineNode = page_doc.Descendants(page_namespace + "Outline").FirstOrDefault();
                                var OutlineNodeID = outlineNode.Attribute("objectID").Value;

                                // 获取outline内部的每个子节点，并以XML方式操作
                                List<XNode> outlineXml = new List<XNode>();
                                foreach (XNode child_node in outlineNode.Nodes())
                                {
                                        outlineXml.Add(child_node);
                                }

                                var OEChildrenNode = "";

                                // 遍历每个节点，获取<OE>节点内容
                                for (int i = 0; i < outlineXml.Count(); i++)
                                {
                                        var outline_childnode = (XElement)outlineXml[i];
                                        var nodename = outline_childnode.Name.LocalName;
                                        if (nodename != "OEChildren")
                                        {
                                                continue;
                                        }
                                        else
                                        {
                                                OEChildrenNode = outline_childnode.ToString();
                                        }
                                }
                                {
                                        string json;
                                        List<server_info> server_infomation;
                                        // 对于大数据，先认证，再使用socket传数据
                                        if (post == null)
                                                post = new PostSubmitter();
                                        post.Url = add_projectcontent_url;
                                        post.PostItems.Clear();
                                        post.PostItems.Add("username", username);
                                        post.PostItems.Add("passwd", password);
                                        post.PostItems.Add("project_code", "Exon");
                                        post.Type = PostSubmitter.PostTypeEnum.Post;
                                        result = post.Post();

                                        if (result.Length > 32){
                                                if (js == null)
                                                        js = new JavaScriptSerializer();
                                                json = result.Replace("\"", "'");
                                                json = "[" + json + "]";
                                                server_infomation = js.Deserialize<List<server_info>>(json);

                                                if (server_infomation.Count() > 0)
                                                {
                                                        DataExchange ThreadClass = new DataExchange(OEChildrenNode, server_infomation[0].host,
                                                                int.Parse(server_infomation[0].port), server_infomation[0].number);
                                                        Thread aThread = new Thread(new ThreadStart(ThreadClass.Send_Content));
                                                        aThread.Start();
                                                }
                                        }
                                }

                        }
                }

		private void isfinished_CheckedChanged(object sender, EventArgs e)
		{
			int i;
			int j = 0;      //加载任务
			int m = -1;
			int chancel = -1;
			CheckBox currentButton = (CheckBox)sender;
			List<string> ls;
			ls = s1;
			CheckedListBox tanfser;
			int index = 0;
			string[] kk = ls.ToArray();

			if (currentButton.Checked)
			{
				foreach (Control aControl in bigPanel.Controls)
				{
					if (aControl is CheckedListBox)
					{
						tanfser = (CheckedListBox)aControl;
						if (index == 0)
						{
							int son_num = tasks[j].Count;
							for (i = 0; i < son_num; i++)
							{
								m = tasks[j][i].Status.IndexOf("完成");
								if (m != -1)
								{
									tanfser.Items.Add(tasks[j][i].ID + " " + tasks[j][i].Name + " " + tasks[j][i].Status);
								}

							}
							j++;
						}

					}
				}
			}
			else
			{
				foreach (Control aControl in bigPanel.Controls)
				{
					if (aControl is CheckedListBox)
					{
						tanfser = (CheckedListBox)aControl;
						for (i = 0; i < tanfser.Items.Count; i++)
						{
							if (tanfser.GetItemChecked(i) == false)
							{
								//判断CheckedListBox某行中是否存在某个特定的字符串
								chancel = tanfser.Items[i].ToString().IndexOf("完成");
								if (chancel != -1)
								{
									tanfser.Items.RemoveAt(i);
									i--;
								}
							}
						}
					}
				}
			}
		}

		 private void isdoing_CheckedChanged(object sender, EventArgs e)
		{
			int i;
			int j = 0;      //加载任务
			int m = -1;
			int chancel = -1;
			CheckBox currentButton = (CheckBox)sender;
			List<string> ls;
			ls = s1;
			CheckedListBox tanfser;
			int index = 0;
			string[] kk = ls.ToArray();

			if (currentButton.Checked)
			{
				foreach (Control aControl in bigPanel.Controls)
				{
					if (aControl is CheckedListBox)
					{
						tanfser = (CheckedListBox)aControl;
						if (index == 0)
						{
							int son_num = tasks[j].Count;
							for (i = 0; i < son_num; i++)
							{
								m = tasks[j][i].Status.IndexOf("进行");
								if (m != -1)
								{
									tanfser.Items.Add(tasks[j][i].ID + " " + tasks[j][i].Name + " " + tasks[j][i].Status);
								}

							}
							j++;
						}

					}
				}
			}
			else
			{
				foreach (Control aControl in bigPanel.Controls)
				{
					if (aControl is CheckedListBox)
					{
						tanfser = (CheckedListBox)aControl;
						for (i = 0; i < tanfser.Items.Count; i++)
						{
							if (tanfser.GetItemChecked(i) == false)
							{
								//判断CheckedListBox某行中是否存在某个特定的字符串
								chancel = tanfser.Items[i].ToString().IndexOf("进行");
								if (chancel != -1)
								{
									tanfser.Items.RemoveAt(i);
									i--;
								}
							}
						}
					}
				}
			}
		}

                private void all_CheckedChanged(object sender, EventArgs e)
                {
			int i;
			int j = 0;
			CheckBox currentButton = (CheckBox)sender;
			List<string> ls;
			ls = s1;
			CheckedListBox tanfser;
			int index = 0;
			string[] kk = ls.ToArray();		//将容器的string集合转化成string数组


			foreach (Control aControl in bigPanel.Controls)
			{
				if (aControl is CheckedListBox)
				{
					tanfser = (CheckedListBox)aControl;
					for (i = 0; i < tanfser.Items.Count; i++)
					{
						if (tanfser.GetItemChecked(i) == false)
						{
							tanfser.Items.RemoveAt(i);
							i--;
						}
					}
				}
			}

			if (currentButton.Checked)
			{
				foreach (Control aControl in bigPanel.Controls)
				{
					if (aControl is CheckedListBox)
					{
						tanfser = (CheckedListBox)aControl;
						if (index == 0)
						{
							int son_num = tasks[j].Count;
							for (i = 0; i < son_num; i++)
							{
								tanfser.Items.Add(tasks[j][i].ID + " " + tasks[j][i].Name + " " + tasks[j][i].Status);

							}
							j++;
						}

					}
				}
			}
			else
			{
				foreach (Control aControl in bigPanel.Controls)
				{
					if (aControl is CheckedListBox)
					{
						tanfser = (CheckedListBox)aControl;
						for (i = 0; i < tanfser.Items.Count; i++)
						{
							//判断CheckedListBox复选框是否选中
							if (tanfser.GetItemChecked(i) == false)
							{
								//删除某行
								tanfser.Items.RemoveAt(i);
								i--;
							}
						}
					}
				}
			}
                }

                private void OneNoteSync_Load(object sender, EventArgs e)
                {

                }

		private void choose_CheckedChanged(object sender, EventArgs e)
		{
			CheckBox currentButton = (CheckBox)sender;
			CheckedListBox replace;

			if (currentButton.Checked)			//复选框是否选中，点击复选框时无论选中否都会执行这个函数，但是该判断语句是判断复选框是狗选中
			{
				foreach (Control aControl in bigPanel.Controls)
				{
					if (aControl is CheckedListBox)
					{
						if (aControl.Name == currentButton.Name)
						{
							replace = (CheckedListBox)aControl;

							//将复选框全部选中

							for (int i = 0; i < replace.Items.Count; i++)
							{
								replace.SetItemChecked(i, true);
							}
						}
					}
				}
			}
		}

		private void button2_Click_1(object sender, EventArgs e)
		{
			Button currentButton = (Button)sender;
			int i, x, y, find;
			int record_content = 0;
			int record_content1 = 0;
			int m = 1;
			int c = 1;
			int k = 0;

			List<int> ls;
			ls = recorder;
			int[] kk;
			CheckedListBox a;

			if (record == 0)
			{
				record_content = Convert.ToInt32(currentButton.Name);
				recorder.Add(record_content);
				record = 1;
			}

			kk = ls.ToArray();

			record_content = Convert.ToInt32(currentButton.Name);
			for (i = 0; i < kk.Length; i++)
			{
				if (record_content == kk[i])
				{

					m = 2;
				}
			}

			if (m == 1)
			{

				recorder.Add(record_content);

			}


			foreach (Control list in bigPanel.Controls)
			{
				if (list is CheckedListBox)
				{
					if (list.Name == currentButton.Name)
					{
						if (list.Visible == true)
							flag = 1;
						else
							flag = 0;
						break;
					}
				}
			}



			kk = ls.ToArray();
			for (i = 0; i < kk.Length; i++) {
				record_content = Convert.ToInt32(currentButton.Name);
				if (record_content == kk[i]) {
					if (flag == 0) {
						foreach (Control son in bigPanel.Controls) {
							if (son is CheckedListBox)
								if (son.Name == currentButton.Name) {
									foreach (Control f in bigPanel.Controls) {
										if (f is TextBox) 
											if ((f.Name == currentButton.Name)) {
												son.Location = new Point(son.Location.X, f.Bottom);
												son.Visible = true;
												break;
											}
									}
									break;
								}
						}

						for (i = (Convert.ToInt32(currentButton.Name) + 1); i < 10; i++) {
							foreach (Control aControl in bigPanel.Controls) {
								if (aControl is TextBox) {
									find = Convert.ToInt32(aControl.Name);
									if ((find) == i) {
										x = aControl.Location.X;
										y = aControl.Location.Y;
										aControl.Location = new Point(x, y + 84);
									}
								}

								if (aControl is CheckBox) {
									int.TryParse(aControl.Name, out find);
									if ((find) == i) {
										x = aControl.Location.X;
										y = aControl.Location.Y;
										foreach (Control son in bigPanel.Controls) {
											if (son.Name == currentButton.Name)
												if (son is CheckedListBox) {
													aControl.Location = new Point(x, y + son.Height);
												}
										}
									}
								}

								if (aControl is Button) {
									int.TryParse(aControl.Name, out find);
									if ((find) == i) {
										x = aControl.Location.X;
										y = aControl.Location.Y;
										foreach (Control son in bigPanel.Controls) {
											if (son.Name == currentButton.Name)
												if (son is CheckedListBox) {
													aControl.Location = new Point(x, y + son.Height);
												}
										}
									}
								}
							}

						}



						foreach (Control aControl in bigPanel.Controls) {
							if (aControl is CheckedListBox) {
								record_content = Convert.ToInt32(currentButton.Name);
								record_content1 = Convert.ToInt32(aControl.Name);
								for (i = 0; i < kk.Length; i++) {
									if ((aControl.Visible == true) && (record_content1 > record_content)) {
										x = aControl.Location.X;
										y = aControl.Location.Y;
										aControl.Location = new Point(x, y + 84);
										break;
									}
								}
							}
						}
					} else {
						foreach (Control son in bigPanel.Controls) {
							if (son.Name == currentButton.Name)
								if (son is CheckedListBox) {
									son.Visible = false;
									break;
								}
						}
						for (i = (Convert.ToInt32(currentButton.Name) + 1); i < 10; i++) {
							foreach (Control aControl in bigPanel.Controls) {
								if (aControl is TextBox) {
									find = Convert.ToInt32(aControl.Name);
									if ((find) == i) {
										x = aControl.Location.X;
										y = aControl.Location.Y;
										aControl.Location = new Point(x, y - 84);
									}
								}



								if (aControl is CheckBox)
								{
									int.TryParse(aControl.Name, out find);
									if ((find) == i)
									{

										x = aControl.Location.X;
										y = aControl.Location.Y;
										foreach (Control son in bigPanel.Controls)
										{
											if (son.Name == currentButton.Name)
												if (son is CheckedListBox)
												{
													aControl.Location = new Point(x, y - son.Height);

												}
										}
									}
								}



								if (aControl is Button)
								{
									int.TryParse(aControl.Name, out find);
									if ((find) == i)
									{

										x = aControl.Location.X;
										y = aControl.Location.Y;
										foreach (Control son in bigPanel.Controls)
										{
											if (son.Name == currentButton.Name)
												if (son is CheckedListBox)
												{
													aControl.Location = new Point(x, y - son.Height);

												}
										}
									}
								}
							}
						}


						foreach (Control aControl in bigPanel.Controls)
						{
							if (aControl is CheckedListBox)
							{

								record_content = Convert.ToInt32(currentButton.Name);
								record_content1 = Convert.ToInt32(aControl.Name);
								for (i = 0; i < kk.Length; i++)
								{
									if ((aControl.Visible == true) && (record_content1 > record_content))
									{
										x = aControl.Location.X;
										y = aControl.Location.Y;

										aControl.Location = new Point(x, y - 84);
										break;
									}
								}
							}
						}
					}
					break;


				}
			}

		}

		private void checkedListBox1_ItemCheck(object sender, ItemCheckEventArgs e)
		{
 			int mun = 1;
			CheckedListBox currentButton = (CheckedListBox)sender;
			List<string> ls;
			ls = project_name;
			string[] kk = ls.ToArray();

			//将选中的那行压入容器中
			s.Add(currentButton.Items[e.Index].ToString());

			if (num == 0) {
				foreach (Control aControl in bigPanel.Controls) {
					if (aControl is TextBox)
					{
						if (aControl.Name == currentButton.Name)
						{
							project_name.Add(aControl.Text);
						}
					}
				}
				num = 1;
			} else {
				foreach (Control aControl in bigPanel.Controls) {
					if (aControl is TextBox) {
						if (aControl.Name == currentButton.Name) {
							for (int i = 0; i < kk.Length; i++) {
								if (kk[i] == aControl.Text) {
									mun = 0;
									break;
								}
							}
							if (mun == 1) {
								project_name.Add(aControl.Text);
								break;
							}
						}
					}
				}
			}
		}

		private void button1_Click(object sender, EventArgs e)
		{
			string fullPath = "C:\\tasks\\qinxiu\\Desktop\\5.txt";
			string[] kk = s.ToArray();			//将容器中的string集合转化成string数组
                        for (int i = 0; i < s.ToArray().Length; i++)
                        {
				if (System.IO.File.Exists(fullPath)){
					//将容器中的字符写入文本文件中
					StreamWriter wr = new StreamWriter(new FileStream(fullPath, FileMode.Append));
                                        wr.WriteLine(s.ToArray()[i]);
					wr.Flush();
					wr.Close();
				}
			}

                        kk = project_name.ToArray();			//将容器中的string集合转化成string数组
			for (int i = 0; i < kk.Length; i++){
				if (System.IO.File.Exists(fullPath)){
					//将容器中的字符写入文本文件中
					StreamWriter wr = new StreamWriter(new FileStream(fullPath, FileMode.Append));
					wr.WriteLine(kk[i]);
					wr.Flush();
					wr.Close();
				}
			}
		}
        }
}