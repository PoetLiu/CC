-- phpMyAdmin SQL Dump
-- version 3.4.11.1deb2+deb7u1
-- http://www.phpmyadmin.net
--
-- 主机: localhost
-- 生成日期: 2015 年 05 月 13 日 17:00
-- 服务器版本: 5.5.40
-- PHP 版本: 5.4.36-0+deb7u1

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- 数据库: `OneNoteSync`
--
CREATE DATABASE `OneNoteSync` DEFAULT CHARACTER SET utf8 COLLATE utf8_bin;
USE `OneNoteSync`;

-- --------------------------------------------------------

--
-- 表的结构 `Notebook`
--

CREATE TABLE IF NOT EXISTS `Notebook` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(256) COLLATE utf8_bin NOT NULL,
  `nID` varchar(256) COLLATE utf8_bin NOT NULL,
  `uID` int(11) NOT NULL,
  `lastmodifiedtime` varchar(256) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=14 ;

--
-- 转存表中的数据 `Notebook`
--

INSERT INTO `Notebook` (`ID`, `name`, `nID`, `uID`, `lastmodifiedtime`) VALUES
(12, '我的笔记本', '{C75E528F-2042-48C1-AC45-51A4AACDB9C4}{1}{B0}', 1, '2015-05-13T07:41:44.000Z'),
(13, 'work', '{3C60AE6D-065A-4AAF-8D9B-D1DAAA0E80BF}{1}{B0}', 1, '2015-04-20T02:07:54.000Z');

-- --------------------------------------------------------

--
-- 表的结构 `Page`
--

CREATE TABLE IF NOT EXISTS `Page` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(256) COLLATE utf8_bin NOT NULL,
  `pID` varchar(256) COLLATE utf8_bin NOT NULL,
  `sID` int(11) NOT NULL,
  `lastModifiedTime` varchar(256) COLLATE utf8_bin NOT NULL,
  `Content` longtext COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=194 ;

--
-- 转存表中的数据 `Page`
--

INSERT INTO `Page` (`ID`, `name`, `pID`, `sID`, `lastModifiedTime`, `Content`) VALUES
(190, 'OneNote: 一站式笔记管理平台', '{3151DD73-3D16-0C9A-3EDC-13F6F23B8A3E}{1}{B0}', 14, '2015-05-13T08:26:40.000Z', ''),
(191, 'OneNote 基础', '{9B316E54-F365-07A7-1875-4434104E48F6}{1}{B0}', 14, '2015-05-13T08:26:42.000Z', ''),
(192, '块 1', '{70A86950-88A1-456A-BCE9-5F59C91D2AA3}{1}{B0}', 15, '2015-04-20T02:07:54.000Z', ''),
(193, '无标题页', '{7F9C74A7-E82B-4FA3-AC3F-AE51CF240FDF}{1}{B0}', 16, '2015-04-20T01:10:00.000Z', '');

-- --------------------------------------------------------

--
-- 表的结构 `Section`
--

CREATE TABLE IF NOT EXISTS `Section` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(256) COLLATE utf8_bin NOT NULL,
  `sID` varchar(256) COLLATE utf8_bin NOT NULL,
  `nID` int(11) NOT NULL,
  `lastModifiedTime` varchar(256) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=17 ;

--
-- 转存表中的数据 `Section`
--

INSERT INTO `Section` (`ID`, `name`, `sID`, `nID`, `lastModifiedTime`) VALUES
(14, '什么', '{65AB35D8-2F71-06CA-37DD-2B1AB1C071B5}{1}{B0}', 12, '2015-05-13T08:26:42.000Z'),
(15, 'yucca', '{36E9D537-B254-4C54-916C-1AA76BB1DFE4}{1}{B0}', 13, '2015-04-20T02:07:54.000Z'),
(16, 'MySHworks', '{5E559E58-7DFD-4A3C-BC56-9F810C1196B8}{1}{B0}', 13, '2015-04-20T01:10:01.000Z');

-- --------------------------------------------------------

--
-- 表的结构 `SyncClient`
--

CREATE TABLE IF NOT EXISTS `SyncClient` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `Number` varchar(32) COLLATE utf8_bin NOT NULL,
  `ClientIP` varchar(32) COLLATE utf8_bin NOT NULL,
  `Time` datetime NOT NULL,
  `IsUse` int(11) NOT NULL DEFAULT '1' COMMENT '是否可用：1 可用，0 不可用',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=21 ;

--
-- 转存表中的数据 `SyncClient`
--

INSERT INTO `SyncClient` (`ID`, `Number`, `ClientIP`, `Time`, `IsUse`) VALUES
(16, 'DRJXEywvrRuo7XCOi839tRHaZGZFQMHT', '192.168.248.112', '2015-05-13 04:21:15', 1),
(17, 'zVf9oJPakOsrGDsQDrQFCpqxJgiPXYYW', '192.168.248.112', '2015-05-13 04:24:38', 1),
(18, 'pR6cN6umLe5ZXTNeTUjQ98sNq4Kn1DfQ', '192.168.248.112', '2015-05-13 04:25:20', 1),
(19, 'uL1j3Usm8QHjtQoEcmQzNLhWQnlPwv9q', '192.168.248.112', '2015-05-13 04:29:41', 1),
(20, 'mgotyqELigYiOp7XCmDbFGXtRd1MaDBW', '192.168.248.112', '2015-05-13 04:53:26', 1);

-- --------------------------------------------------------

--
-- 表的结构 `SyncPage`
--

CREATE TABLE IF NOT EXISTS `SyncPage` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `SyncClientID` int(11) NOT NULL,
  `PageID` int(11) NOT NULL,
  `pID` varchar(256) COLLATE utf8_bin NOT NULL,
  `Kind` int(11) NOT NULL COMMENT '操作类型： 0 插入新内容； 1 更新内容； 2 下载内容',
  `IsUse` int(11) NOT NULL DEFAULT '1' COMMENT '是否可用：1 可用，0 不可用。',
  PRIMARY KEY (`ID`),
  UNIQUE KEY `ID` (`ID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=6 ;

--
-- 转存表中的数据 `SyncPage`
--

INSERT INTO `SyncPage` (`ID`, `SyncClientID`, `PageID`, `pID`, `Kind`, `IsUse`) VALUES
(2, 16, 190, '{3151DD73-3D16-0C9A-3EDC-13F6F23B8A3E}{1}{B0}', 0, 1),
(3, 16, 191, '{9B316E54-F365-07A7-1875-4434104E48F6}{1}{B0}', 0, 1),
(4, 16, 192, '{70A86950-88A1-456A-BCE9-5F59C91D2AA3}{1}{B0}', 0, 1),
(5, 16, 193, '{7F9C74A7-E82B-4FA3-AC3F-AE51CF240FDF}{1}{B0}', 0, 1);

-- --------------------------------------------------------

--
-- 表的结构 `User`
--

CREATE TABLE IF NOT EXISTS `User` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(256) COLLATE utf8_bin NOT NULL,
  `user` varchar(256) COLLATE utf8_bin NOT NULL,
  `passwd` varchar(256) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 COLLATE=utf8_bin AUTO_INCREMENT=2 ;

--
-- 转存表中的数据 `User`
--

INSERT INTO `User` (`ID`, `name`, `user`, `passwd`) VALUES
(1, '文朝', 'wenchao', 'weiwenchao');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
