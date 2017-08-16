namespace NoteSync
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
                        this.label1 = new System.Windows.Forms.Label();
                        this.label2 = new System.Windows.Forms.Label();
                        this.user_text = new System.Windows.Forms.TextBox();
                        this.passwd_text = new System.Windows.Forms.TextBox();
                        this.LoginButton = new System.Windows.Forms.Button();
                        this.SuspendLayout();
                        // 
                        // label1
                        // 
                        this.label1.AutoSize = true;
                        this.label1.Location = new System.Drawing.Point(27, 40);
                        this.label1.Name = "label1";
                        this.label1.Size = new System.Drawing.Size(53, 12);
                        this.label1.TabIndex = 0;
                        this.label1.Text = "用户名：";
                        // 
                        // label2
                        // 
                        this.label2.AutoSize = true;
                        this.label2.Location = new System.Drawing.Point(27, 80);
                        this.label2.Name = "label2";
                        this.label2.Size = new System.Drawing.Size(35, 12);
                        this.label2.TabIndex = 0;
                        this.label2.Text = "密码:";
                        // 
                        // user_text
                        // 
                        this.user_text.Location = new System.Drawing.Point(98, 30);
                        this.user_text.Name = "user_text";
                        this.user_text.Size = new System.Drawing.Size(130, 21);
                        this.user_text.TabIndex = 1;
                        // 
                        // passwd_text
                        // 
                        this.passwd_text.Location = new System.Drawing.Point(98, 80);
                        this.passwd_text.Name = "passwd_text";
                        this.passwd_text.Size = new System.Drawing.Size(130, 21);
                        this.passwd_text.TabIndex = 1;
                        // 
                        // LoginButton
                        // 
                        this.LoginButton.Location = new System.Drawing.Point(153, 129);
                        this.LoginButton.Name = "LoginButton";
                        this.LoginButton.Size = new System.Drawing.Size(75, 23);
                        this.LoginButton.TabIndex = 2;
                        this.LoginButton.Text = "登录";
                        this.LoginButton.UseVisualStyleBackColor = true;
                        this.LoginButton.Click += new System.EventHandler(this.LoginButton_Click);
                        // 
                        // Login
                        // 
                        this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
                        this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
                        this.ClientSize = new System.Drawing.Size(244, 166);
                        this.Controls.Add(this.LoginButton);
                        this.Controls.Add(this.passwd_text);
                        this.Controls.Add(this.user_text);
                        this.Controls.Add(this.label2);
                        this.Controls.Add(this.label1);
                        this.MaximizeBox = false;
                        this.MinimizeBox = false;
                        this.Name = "Login";
                        this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
                        this.Text = "Login";
                        this.ResumeLayout(false);
                        this.PerformLayout();

                }

                #endregion

                private System.Windows.Forms.Label label1;
                private System.Windows.Forms.Label label2;
                private System.Windows.Forms.TextBox user_text;
                private System.Windows.Forms.TextBox passwd_text;
                private System.Windows.Forms.Button LoginButton;
        }
}