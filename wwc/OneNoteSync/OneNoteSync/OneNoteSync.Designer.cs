using System.ComponentModel;
using System.IO;
namespace OneNoteSync
{
        public partial class OneNoteSync
        {
                /// <summary>
                /// Required designer variable.
                /// </summary>

                private System.ComponentModel.IContainer components = null;
                Login pLogin;
		string username;
		string password;
		string[] str1 = new string[6];
                /// <summary>
                /// Clean up any resources being used.
                /// </summary>
                /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
                protected override void Dispose(bool disposing)
                {
                        if (disposing && (components != null))
                        {
                                components.Dispose();
                        }
                        base.Dispose(disposing);
                }

                public OneNoteSync(Login _pLogin, string _user, string _passwd,string []str):this()
                {
                        this.pLogin = _pLogin;
			username = _user;
			password = _passwd;
			str1 = str;

                }

                protected override void OnClosing(CancelEventArgs e)
                {
                        this.pLogin.Close();
                }

                #region Windows Form Designer generated code

                /// <summary>
                /// Required method for Designer support - do not modify
                /// the contents of this method with the code editor.
                /// </summary>
                private void InitializeComponent()
                {
                        this.isAbort = new System.Windows.Forms.CheckBox();
                        this.isfinished = new System.Windows.Forms.CheckBox();
                        this.isdoing = new System.Windows.Forms.CheckBox();
                        this.all = new System.Windows.Forms.CheckBox();
                        this.download = new System.Windows.Forms.Button();
                        this.sync = new System.Windows.Forms.Button();
                        this.SuspendLayout();
                        // 
                        // isAbort
                        // 
                        this.isAbort.AutoSize = true;
                        this.isAbort.Location = new System.Drawing.Point(26, 13);
                        this.isAbort.Name = "isAbort";
                        this.isAbort.Size = new System.Drawing.Size(48, 16);
                        this.isAbort.TabIndex = 0;
                        this.isAbort.Text = "中止";
                        this.isAbort.UseVisualStyleBackColor = true;
                        this.isAbort.CheckedChanged += new System.EventHandler(this.isAbort_CheckedChanged);
                        // 
                        // isfinished
                        // 
                        this.isfinished.AutoSize = true;
                        this.isfinished.Location = new System.Drawing.Point(118, 13);
                        this.isfinished.Name = "isfinished";
                        this.isfinished.Size = new System.Drawing.Size(48, 16);
                        this.isfinished.TabIndex = 1;
                        this.isfinished.Text = "完成";
                        this.isfinished.UseVisualStyleBackColor = true;
			this.isfinished.CheckedChanged += new System.EventHandler(this.isfinished_CheckedChanged);
                        // 
                        // isdoing
                        // 
                        this.isdoing.AutoSize = true;
                        this.isdoing.Location = new System.Drawing.Point(246, 13);
                        this.isdoing.Name = "isdoing";
                        this.isdoing.Size = new System.Drawing.Size(60, 16);
                        this.isdoing.TabIndex = 2;
                        this.isdoing.Text = "进行中";
                        this.isdoing.UseVisualStyleBackColor = true;
			this.isdoing.CheckedChanged += new System.EventHandler(this.isdoing_CheckedChanged);

                        // 
                        // all
                        // 
                        this.all.AutoSize = true;
                        this.all.Location = new System.Drawing.Point(371, 13);
                        this.all.Name = "all";
                        this.all.Size = new System.Drawing.Size(48, 16);
                        this.all.TabIndex = 3;
                        this.all.Text = "全部";
                        this.all.UseVisualStyleBackColor = true;
                        this.all.CheckedChanged += new System.EventHandler(this.all_CheckedChanged);
                        // 
                        // download
                        // 
                        this.download.Location = new System.Drawing.Point(371, 66);
                        this.download.Name = "download";
                        this.download.Size = new System.Drawing.Size(75, 23);
                        this.download.TabIndex = 4;
                        this.download.Text = "下载";
                        this.download.UseVisualStyleBackColor = true;
                        this.download.Click += new System.EventHandler(this.download_Click);
                        // 
                        // sync
                        // 
                        this.sync.Location = new System.Drawing.Point(371, 109);
                        this.sync.Name = "sync";
                        this.sync.Size = new System.Drawing.Size(75, 23);
                        this.sync.TabIndex = 4;
                        this.sync.Text = "同步";
                        this.sync.UseVisualStyleBackColor = true;
                        this.sync.Click += new System.EventHandler(this.sync_Click);
                        // 
                        // OneNoteSync
                        // 
                        this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
                        this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
                        this.AutoSize = true;
                        this.ClientSize = new System.Drawing.Size(499, 295);
                        this.Controls.Add(this.sync);
                        this.Controls.Add(this.download);
                        this.Controls.Add(this.all);
                        this.Controls.Add(this.isdoing);
                        this.Controls.Add(this.isfinished);
                        this.Controls.Add(this.isAbort);
                        this.MaximizeBox = false;
                        this.MinimizeBox = false;
                        this.Name = "OneNoteSync";
                        this.Text = "OneNoteSync";
                        this.Load += new System.EventHandler(this.OneNoteSync_Load);
                        this.ResumeLayout(false);
                        this.PerformLayout();

			this.Load += new System.EventHandler(this.Form1_Load);

                }

                #endregion

                private System.Windows.Forms.CheckBox isAbort;
                private System.Windows.Forms.CheckBox isfinished;
                private System.Windows.Forms.CheckBox isdoing;
                private System.Windows.Forms.CheckBox all;
                private System.Windows.Forms.Button download;
                private System.Windows.Forms.Button sync;
        }
}

