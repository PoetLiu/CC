<?php
/*
 * url router
 */
include "./OneNoteSync.php";

header("Content-type: text/html; charset=utf-8");
session_start();
$path = array_keys($_GET);
$routeParts  = split("/", $path[0]);
$action = $routeParts[0];   // 控制器

$Sync   = new OneNoteSync();
$Sync->$action();
?>


