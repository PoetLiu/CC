namespace OneNoteSync
{
        partial class Login
        {
                /// <summary>
                /// Required designer variable.
                /// </summary>
                private System.ComponentModel.IContainer components = null;

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

                #region Windows Form Designer generated code

                /// <summary>
                /// Required method for Designer support - do not modify
                /// the contents of this method with the code editor.
                /// </summary>
                private void InitializeComponent()
                {
			this.user = new System.Windows.Forms.TextBox();
			this.passwd = new System.Windows.Forms.TextBox();
			this.user_name = new System.Windows.Forms.Label();
			this.user_passwd = new System.Windows.Forms.Label();
			this.sure = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// user
			// 
			this.user.Location = new System.Drawing.Point(204, 18);
			this.user.Margin = new System.Windows.Forms.Padding(4);
			this.user.Name = "user";
			this.user.Size = new System.Drawing.Size(244, 28);
			this.user.TabIndex = 0;
			this.user.TextChanged += new System.EventHandler(this.user_TextChanged);
			// 
			// passwd
			// 
			this.passwd.Location = new System.Drawing.Point(204, 80);
			this.passwd.Margin = new System.Windows.Forms.Padding(4);
			this.passwd.Name = "passwd";
			this.passwd.Size = new System.Drawing.Size(244, 28);
			this.passwd.TabIndex = 1;
			this.passwd.TextChanged += new System.EventHandler(this.passwd_TextChanged);
			// 
			// user_name
			// 
			this.user_name.AutoSize = true;
			this.user_name.Location = new System.Drawing.Point(42, 18);
			this.user_name.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.user_name.Name = "user_name";
			this.user_name.Size = new System.Drawing.Size(62, 18);
			this.user_name.TabIndex = 2;
			this.user_name.Text = "用户名";
			this.user_name.TextAlign = System.Drawing.ContentAlignment.TopCenter;
			// 
			// user_passwd
			// 
			this.user_passwd.AutoSize = true;
			this.user_passwd.Location = new System.Drawing.Point(42, 80);
			this.user_passwd.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
			this.user_passwd.Name = "user_passwd";
			this.user_passwd.Size = new System.Drawing.Size(44, 18);
			this.user_passwd.TabIndex = 2;
			this.user_passwd.Text = "密码";
			// 
			// sure
			// 
			this.sure.Location = new System.Drawing.Point(465, 122);
			this.sure.Margin = new System.Windows.Forms.Padding(4);
			this.sure.Name = "sure";
			this.sure.Size = new System.Drawing.Size(112, 34);
			this.sure.TabIndex = 3;
			this.sure.Text = " 确定";
			this.sure.UseVisualStyleBackColor = true;
			this.sure.Click += new System.EventHandler(this.sure_Click);
			// 
			// Login
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 18F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(596, 154);
			this.Controls.Add(this.sure);
			this.Controls.Add(this.user_passwd);
			this.Controls.Add(this.user_name);
			this.Controls.Add(this.passwd);
			this.Controls.Add(this.user);
			this.Margin = new System.Windows.Forms.Padding(4);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "Login";
			this.Text = "Login";
			this.Load += new System.EventHandler(this.Login_Load);
			this.ResumeLayout(false);
			this.PerformLayout();

                }

                #endregion

                private System.Windows.Forms.TextBox user;
                private System.Windows.Forms.TextBox passwd;
                private System.Windows.Forms.Label user_name;
                private System.Windows.Forms.Label user_passwd;
		private System.Windows.Forms.Button sure;
        }
}