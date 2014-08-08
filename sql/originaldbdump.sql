-- MySQL dump 10.13  Distrib 5.5.38, for Linux (x86_64)
--
-- Host: localhost    Database: gigadash
-- ------------------------------------------------------
-- Server version	5.5.38

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `asset_cat`
--

DROP TABLE IF EXISTS `asset_cat`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `asset_cat` (
  `asset_id` int(11) NOT NULL AUTO_INCREMENT,
  `asset_type` varchar(15) NOT NULL,
  PRIMARY KEY (`asset_id`),
  UNIQUE KEY `asset_id` (`asset_id`),
  UNIQUE KEY `asset_type` (`asset_type`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `asset_cat`
--

LOCK TABLES `asset_cat` WRITE;
/*!40000 ALTER TABLE `asset_cat` DISABLE KEYS */;
INSERT INTO `asset_cat` VALUES (1,'server');
/*!40000 ALTER TABLE `asset_cat` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `ci_cat`
--

DROP TABLE IF EXISTS `ci_cat`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `ci_cat` (
  `type_id` int(11) NOT NULL AUTO_INCREMENT,
  `ci_type` varchar(15) NOT NULL,
  PRIMARY KEY (`type_id`),
  UNIQUE KEY `type_id` (`type_id`),
  UNIQUE KEY `ci_type` (`ci_type`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ci_cat`
--

LOCK TABLES `ci_cat` WRITE;
/*!40000 ALTER TABLE `ci_cat` DISABLE KEYS */;
INSERT INTO `ci_cat` VALUES (1,'Hard Drive');
/*!40000 ALTER TABLE `ci_cat` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `corp_cat`
--

DROP TABLE IF EXISTS `corp_cat`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `corp_cat` (
  `cat_id` int(11) NOT NULL AUTO_INCREMENT,
  `corp_type` varchar(15) NOT NULL,
  PRIMARY KEY (`cat_id`),
  UNIQUE KEY `cat_id` (`cat_id`),
  UNIQUE KEY `corp_type` (`corp_type`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `corp_cat`
--

LOCK TABLES `corp_cat` WRITE;
/*!40000 ALTER TABLE `corp_cat` DISABLE KEYS */;
INSERT INTO `corp_cat` VALUES (1,'dcp');
/*!40000 ALTER TABLE `corp_cat` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `dc_providers`
--

DROP TABLE IF EXISTS `dc_providers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `dc_providers` (
  `company_name` varchar(30) NOT NULL,
  `rg_shortname` varchar(3) NOT NULL,
  `pr_shortname` varchar(6) NOT NULL,
  `cp_code` varchar(3) DEFAULT NULL,
  `rh_contact` varchar(50) NOT NULL,
  `rh_portal` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`pr_shortname`),
  UNIQUE KEY `pr_shortname` (`pr_shortname`),
  UNIQUE KEY `rh_contact` (`rh_contact`),
  KEY `company_name` (`company_name`),
  KEY `rg_shortname` (`rg_shortname`),
  KEY `cp_code` (`cp_code`),
  CONSTRAINT `dc_providers_ibfk_1` FOREIGN KEY (`company_name`) REFERENCES `external_sources` (`company_name`),
  CONSTRAINT `dc_providers_ibfk_2` FOREIGN KEY (`rg_shortname`) REFERENCES `gd_regions` (`rg_code`),
  CONSTRAINT `dc_providers_ibfk_3` FOREIGN KEY (`cp_code`) REFERENCES `fam_corps` (`cp_code`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dc_providers`
--

LOCK TABLES `dc_providers` WRITE;
/*!40000 ALTER TABLE `dc_providers` DISABLE KEYS */;
INSERT INTO `dc_providers` VALUES ('Telecity','AMS','AMSTEL','GN','nl.support.AMS1@telecity.com','updateme');
/*!40000 ALTER TABLE `dc_providers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `external_sources`
--

DROP TABLE IF EXISTS `external_sources`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `external_sources` (
  `company_id` int(11) NOT NULL AUTO_INCREMENT,
  `company_name` varchar(30) NOT NULL,
  `corp_type` varchar(15) NOT NULL,
  `technical_contact` varchar(50) DEFAULT NULL,
  `billing_contact` varchar(50) DEFAULT NULL,
  `company_website` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`company_id`),
  UNIQUE KEY `company_id` (`company_id`),
  UNIQUE KEY `company_name` (`company_name`),
  KEY `corp_type` (`corp_type`),
  CONSTRAINT `external_sources_ibfk_1` FOREIGN KEY (`corp_type`) REFERENCES `corp_cat` (`corp_type`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `external_sources`
--

LOCK TABLES `external_sources` WRITE;
/*!40000 ALTER TABLE `external_sources` DISABLE KEYS */;
INSERT INTO `external_sources` VALUES (1,'Telecity','dcp','nl.support.abp@telecity.com','nl.support.sp@telecity.com','http://support.telecity.net/'),(2,'EQUINIX','dcp','servicedesk.nl@eu.equinix.com',NULL,'https://portal.equinix.com/web-portal/login/login.'),(3,'Net2EZ','dcp','support@netdc.com',NULL,'https://eznoc.net2ez.com/customer/main/index/');
/*!40000 ALTER TABLE `external_sources` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `fam_corps`
--

DROP TABLE IF EXISTS `fam_corps`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `fam_corps` (
  `cp_name` varchar(30) NOT NULL,
  `cp_code` varchar(3) NOT NULL,
  `president` varchar(30) DEFAULT NULL,
  `address` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`cp_name`),
  UNIQUE KEY `cp_name` (`cp_name`),
  UNIQUE KEY `cp_code` (`cp_code`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `fam_corps`
--

LOCK TABLES `fam_corps` WRITE;
/*!40000 ALTER TABLE `fam_corps` DISABLE KEYS */;
INSERT INTO `fam_corps` VALUES ('Data Foundry, Inc.','DF','Shane Menking','updateme'),('Giganews, Inc.','GN','Sunday Yokubaitis','updateme'),('Golden Frog','GF','Sunday Yokubaitis','updateme'),('Powerhouse Mgmt.','PH','Ron Yokubaitis','updateme');
/*!40000 ALTER TABLE `fam_corps` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gd_drives`
--

DROP TABLE IF EXISTS `gd_drives`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gd_drives` (
  `ci_type` varchar(15) DEFAULT NULL,
  `serial_number` varchar(50) NOT NULL,
  `site` varchar(4) DEFAULT NULL,
  `host_shortname` varchar(25) NOT NULL,
  `drive_label` varchar(5) NOT NULL,
  `port` int(11) NOT NULL DEFAULT '0',
  `spool_number` int(11) DEFAULT NULL,
  `size_bytes` int(11) DEFAULT NULL,
  `size_human` varchar(5) DEFAULT NULL,
  `drive_status` varchar(9) DEFAULT NULL,
  `spool_status` varchar(11) DEFAULT NULL,
  `spool_fill` int(11) DEFAULT NULL,
  `pending_sectors` int(11) NOT NULL DEFAULT '0',
  `reallocated_sectors` int(11) NOT NULL DEFAULT '0',
  `raw_read_error_rate` int(11) NOT NULL DEFAULT '0',
  `in_use` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`host_shortname`,`drive_label`,`serial_number`),
  UNIQUE KEY `serial_number` (`serial_number`),
  KEY `ci_type` (`ci_type`),
  KEY `site` (`site`),
  CONSTRAINT `gd_drives_ibfk_1` FOREIGN KEY (`host_shortname`) REFERENCES `gd_servers` (`host_shortname`),
  CONSTRAINT `gd_drives_ibfk_2` FOREIGN KEY (`ci_type`) REFERENCES `ci_cat` (`ci_type`),
  CONSTRAINT `gd_drives_ibfk_3` FOREIGN KEY (`site`) REFERENCES `gd_sites` (`site_name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gd_drives`
--

LOCK TABLES `gd_drives` WRITE;
/*!40000 ALTER TABLE `gd_drives` DISABLE KEYS */;
/*!40000 ALTER TABLE `gd_drives` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gd_regions`
--

DROP TABLE IF EXISTS `gd_regions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gd_regions` (
  `rg_name` varchar(50) NOT NULL,
  `rg_code` varchar(3) NOT NULL,
  `rg_country` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`rg_name`),
  UNIQUE KEY `rg_name` (`rg_name`),
  UNIQUE KEY `rg_code` (`rg_code`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gd_regions`
--

LOCK TABLES `gd_regions` WRITE;
/*!40000 ALTER TABLE `gd_regions` DISABLE KEYS */;
INSERT INTO `gd_regions` VALUES ('Amsterdam','AMS','Netherlands'),('Ashburn','DCA','United States of America'),('Austin','AUS','United States of America'),('Europe','EU2','European Union');
/*!40000 ALTER TABLE `gd_regions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gd_servers`
--

DROP TABLE IF EXISTS `gd_servers`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gd_servers` (
  `asset_type` varchar(15) DEFAULT NULL,
  `hostname` varchar(50) NOT NULL,
  `host_shortname` varchar(25) NOT NULL,
  `uid` int(11) NOT NULL AUTO_INCREMENT,
  `server_type` varchar(15) DEFAULT NULL,
  `site` varchar(4) DEFAULT NULL,
  `location` varchar(50) DEFAULT NULL,
  `mac_addr` varchar(18) NOT NULL,
  `num_drives` int(11) NOT NULL,
  `num_arrays` int(11) NOT NULL,
  `recover_status` varchar(12) DEFAULT NULL,
  `gc_status` varchar(15) DEFAULT NULL,
  `var_fill` int(11) DEFAULT NULL,
  `open_ticket` tinyint(1) NOT NULL DEFAULT '0',
  `last_report` datetime DEFAULT NULL,
  PRIMARY KEY (`hostname`),
  UNIQUE KEY `hostname` (`hostname`),
  UNIQUE KEY `host_shortname` (`host_shortname`),
  UNIQUE KEY `uid` (`uid`),
  UNIQUE KEY `mac_addr` (`mac_addr`),
  KEY `asset_type` (`asset_type`),
  KEY `server_type` (`server_type`),
  KEY `site` (`site`),
  CONSTRAINT `gd_servers_ibfk_1` FOREIGN KEY (`asset_type`) REFERENCES `asset_cat` (`asset_type`),
  CONSTRAINT `gd_servers_ibfk_2` FOREIGN KEY (`server_type`) REFERENCES `server_cat` (`server_type`),
  CONSTRAINT `gd_servers_ibfk_3` FOREIGN KEY (`site`) REFERENCES `gd_sites` (`site_name`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gd_servers`
--

LOCK TABLES `gd_servers` WRITE;
/*!40000 ALTER TABLE `gd_servers` DISABLE KEYS */;
INSERT INTO `gd_servers` VALUES ('Server','r283.gc.ams3.giganews.com','r283.ams3',2,'r_server','AMS3','AMS','00:15:17:fd:79:f0',12,4,'Completed','Running',50,0,'2014-06-21 04:42:19'),('Server','test.ams3.giganews.com','test.ams3',5,'r_server','AMS3','AMS','aa:bb:cc:dd:ee:ff',12,5,'Running','Running',60,0,'0000-00-00 00:00:00');
/*!40000 ALTER TABLE `gd_servers` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gd_sites`
--

DROP TABLE IF EXISTS `gd_sites`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gd_sites` (
  `site_name` varchar(4) NOT NULL,
  `rg_code` varchar(3) NOT NULL,
  `country` varchar(25) DEFAULT NULL,
  `dc_provider` varchar(6) NOT NULL,
  `rh_contact` varchar(50) DEFAULT NULL,
  `dc_address` varchar(50) DEFAULT NULL,
  `zip_code` varchar(7) DEFAULT NULL,
  PRIMARY KEY (`site_name`),
  UNIQUE KEY `site_name` (`site_name`),
  KEY `dc_provider` (`dc_provider`),
  KEY `rh_contact` (`rh_contact`),
  KEY `rg_code` (`rg_code`),
  CONSTRAINT `gd_sites_ibfk_1` FOREIGN KEY (`dc_provider`) REFERENCES `dc_providers` (`pr_shortname`),
  CONSTRAINT `gd_sites_ibfk_2` FOREIGN KEY (`rh_contact`) REFERENCES `dc_providers` (`rh_contact`),
  CONSTRAINT `gd_sites_ibfk_3` FOREIGN KEY (`rg_code`) REFERENCES `gd_regions` (`rg_code`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gd_sites`
--

LOCK TABLES `gd_sites` WRITE;
/*!40000 ALTER TABLE `gd_sites` DISABLE KEYS */;
INSERT INTO `gd_sites` VALUES ('AMS3','AMS','Netherlands','AMSTEL','nl.support.AMS1@telecity.com','updateme','000000');
/*!40000 ALTER TABLE `gd_sites` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gd_subregions`
--

DROP TABLE IF EXISTS `gd_subregions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gd_subregions` (
  `some_stuff` varchar(30) NOT NULL,
  PRIMARY KEY (`some_stuff`),
  UNIQUE KEY `some_stuff` (`some_stuff`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gd_subregions`
--

LOCK TABLES `gd_subregions` WRITE;
/*!40000 ALTER TABLE `gd_subregions` DISABLE KEYS */;
/*!40000 ALTER TABLE `gd_subregions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `server_cat`
--

DROP TABLE IF EXISTS `server_cat`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `server_cat` (
  `type_id` int(11) NOT NULL AUTO_INCREMENT,
  `server_type` varchar(15) NOT NULL,
  PRIMARY KEY (`type_id`),
  UNIQUE KEY `type_id` (`type_id`),
  UNIQUE KEY `server_type` (`server_type`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `server_cat`
--

LOCK TABLES `server_cat` WRITE;
/*!40000 ALTER TABLE `server_cat` DISABLE KEYS */;
INSERT INTO `server_cat` VALUES (1,'r_server');
/*!40000 ALTER TABLE `server_cat` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2014-07-31 22:13:34
