<?php
/*
 * OneNote Sync PHP script
 * author       weiwenchao
 * time         2015-05-03
 * */
include "./MySQLConfig.php";
class OneNoteSync
{
        private $MySQLCon;

        //
        // return:
        //      0
        //      1       error
        private function AuthUser($Username, $passwd)
        {
                $result = mysql_query("select Name from Staff where User='$Username'");
                while($row=mysql_fetch_row($result)){
                                return 0;
                }
                return 1;

        }
        // function of construc : initial MySQL connect
        public function __construct()
        {
                $this->MySQLCon = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASSWD);
                if(!$this->MySQLCon){
                        die('Could not connect: ' . mysql_error());
                }

                $db_selected = mysql_select_db(MYSQL_NAME, $this->MySQLCon);
                if (!$db_selected){
                          die ("Can\'t use database  : " . mysql_error());
                }

                mysql_query("set names utf8");

        }
    // authentification of user
        // return:
        //      0       user name and password are correct
        //      1       user name and password are error
        // output:
        //      yes     user name and password are correct
        //      no      user name and password are error
        public function Login()
        {
                // todo Authorization
                $user   = $_POST['username'];
                $passwd = $_POST['passwd'];

                if($this->AuthUser($user, $passwd)){
                        echo "username or password error!";
                }else{
                        $result = mysql_query("select Name,Sex,Email,Region,Mobile,Status from Staff where User=+'$user'");
			$output	= array();
                        while($row=mysql_fetch_row($result, MYSQL_ASSOC))
				$output[]	= $row;

			echo json_encode($output);
                }
	}

        // get project list by username
        public function get_project()
        {
                // todo Authorization
                $user   = $_POST['username'];
                $passwd = $_POST['passwd'];
                if($this->AuthUser($user, $passwd)){
                         // if($user==""||$passwd==""){
                         echo "no";
                         return 0;
                }
		$result = mysql_query("select Code from Project");
                $c=array();
                while($row=mysql_fetch_row($result, MYSQL_ASSOC))
                        $c[]=$row;
                echo json_encode($c);

        }

        // get task list by project code
        public function get_task()
        {
                // todo Authorization
                $user   	= $_POST['username'];
                $passwd 	= $_POST['passwd'];
		$project_code	= $_POST['project_code'];
                if($this->AuthUser($user, $passwd)){
                        echo "no";
                        return 0;
                }
		if($project_code=="")
			return;
		$sql	= "SELECT Task.ID, Task.Name, Task.Status FROM Task ".
				"LEFT JOIN Project ON Project.ID = Task.project_id WHERE Project.Code =  '$project_code'";
                $result = mysql_query($sql);
		$output	= array();
                while($row=mysql_fetch_array($result, MYSQL_ASSOC)){
			$output[]	= $row;
                }
                echo json_encode($output,JSON_UNESCAPED_UNICODE);    //JSON_UNESCAPED_UNICODE将中文显示

        }
	
	// add project content
	public function add_projectcontent()
        {
                // todo
		$project_code	= $_POST['project_code'];
                $user   	= $_POST['username'];
                $passwd 	= $_POST['passwd'];
                if($this->AuthUser($user, $passwd)){
                        return;
                }
		if($project_code=="")
			return;
                $number = $this->getRandChar(32);

                $output = array();

                $output['host']='192.168.145.131';
                $output['port'] = '3563';
                $output['number']       = $number;

                $time   =  date('y-m-d h:i:s',time());
                $sql    = "insert into Load_data(BaseCode, Kind, Time, isUsed, ProjectCode, TaskCode) values('$number', 
                        '0',  '$time', 0, '$project_code', '')";
                mysql_query($sql);
                echo json_encode($output,JSON_UNESCAPED_UNICODE);    //JSON_UNESCAPED_UNICODE将中文显示
	}

        function getRandChar($length)
        {
                $str = null;
                $strPol = "ABCDEFGHIJKLMNOPQRSTUVWXYZ123456789abcdefghijklmnopqrstuvwxyz";
                $max = strlen($strPol)-1;
                for($i=0;$i<$length;$i++){
                        $str.=$strPol[rand(0,$max)];//rand($min,$max)生成介于min和max两个数之间的一个随机整数                        
                }
                return $str;
        }
        // get project content
	public function get_projectcontent()
	{
		$project_code   = $_POST['project_code'];
                $user   	= $_POST['username'];
                $passwd 	= $_POST['passwd'];
                if($this->AuthUser($user, $passwd)){
                        echo "no";
                        return 0;
                }
		if($project_code=="")
			return;

		$sql	= "SELECT Content FROM ProjectContent where ProjectCode='$project_code'";
		$result	= mysql_query($sql);
		while($row=mysql_fetch_array($result, MYSQL_ASSOC)){
			echo $row["Content"];
			break;
		}
	}

  	function __destruct()
        {
                if($this->MySQLCon)
                        mysql_close($this->MySQLCon);
        }
}
?>

