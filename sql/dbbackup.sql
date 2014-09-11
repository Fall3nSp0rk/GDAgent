-- MySQL dump 10.13  Distrib 5.5.38, for Linux (x86_64)
--
-- Host: localhost    Database: `gigadash` 
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
-- Current Database: `gigadash`
--

/*!40000 DROP DATABASE IF EXISTS `gigadash`*/;

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `gigadash` /*!40100 DEFAULT CHARACTER SET latin1 */;
DROP USER IF EXISTS `GigaDash`@`localhost`;
CREATE USER IF NOT EXISTS `GigaDash`@`localhost` IDENTIFIED BY PASSWORD '*a1c8a7dbca6c36f2a70289f4acd8dec128b09d3c';
GRANT ALL ON `gigadash`.* TO `GigaDash`.`localhost`;
USE `gigadash`;

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
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `asset_cat`
--

LOCK TABLES `asset_cat` WRITE;
/*!40000 ALTER TABLE `asset_cat` DISABLE KEYS */;
INSERT INTO `asset_cat` VALUES (5,'ax'),(4,'pdu'),(2,'router'),(1,'server'),(3,'switch');
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
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `ci_cat`
--

LOCK TABLES `ci_cat` WRITE;
/*!40000 ALTER TABLE `ci_cat` DISABLE KEYS */;
INSERT INTO `ci_cat` VALUES (1,'Hard Drive'),(2,'RAM Module');
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
  `type_desc` varchar(30) NOT NULL,
  PRIMARY KEY (`cat_id`),
  UNIQUE KEY `cat_id` (`cat_id`),
  UNIQUE KEY `corp_type` (`corp_type`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `corp_cat`
--

LOCK TABLES `corp_cat` WRITE;
/*!40000 ALTER TABLE `corp_cat` DISABLE KEYS */;
INSERT INTO `corp_cat` VALUES (2,'car', 'Tier 1 Network Carrier'),(1,'dcp', 'DataCenter Provider'),(5,'isp', 'Tier 2 Network Carrier'),(6,'man', 'Hardware Manufacturer'),(3,'per', 'Peering Partner'),(4,'shc', 'Server Hosting Company');
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
  `srg_code` varchar(3) NOT NULL,
  `pr_shortname` varchar(6) NOT NULL,
  `cp_code` varchar(3) DEFAULT NULL,
  `rh_contact` varchar(50) NOT NULL,
  `rh_portal` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`pr_shortname`),
  UNIQUE KEY `pr_shortname` (`pr_shortname`),
  UNIQUE KEY `rh_contact` (`rh_contact`),
  KEY `company_name` (`company_name`),
  KEY `srg_code` (`srg_code`),
  KEY `cp_code` (`cp_code`),
  CONSTRAINT `dc_providers_ibfk_1` FOREIGN KEY (`company_name`) REFERENCES `external_sources` (`company_name`),
  CONSTRAINT `dc_providers_ibfk_2` FOREIGN KEY (`srg_code`) REFERENCES `gd_subregions` (`srg_code`),
  CONSTRAINT `dc_providers_ibfk_3` FOREIGN KEY (`cp_code`) REFERENCES `fam_corps` (`cp_code`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `dc_providers`
--

LOCK TABLES `dc_providers` WRITE;
/*!40000 ALTER TABLE `dc_providers` DISABLE KEYS */;
INSERT INTO `dc_providers` VALUES ('Equinix','AMS','AMSEQI','GN','Servicedesk.nl@eu.equinix.com','na'),('Telecity','AMS','AMSTEL','GN','nl.support.ams@telecity.com','http://support.telecity.net/'),('Equinix','DCA','DCAEQI','DF','na','https://portal.equinix.com/web-portal/login/login.'),('Net2EZ','DCA','DCAN2E','GN','support@netdc.com ','n/a'),('Terremark','USW','GFMTRM','GF','servicecenter@terremark.com','not available'),('Equinix','HKG','HKGEQI','GN','none','https://esp.ap.equinix.com/servicePortal'),('Net2EZ','USW','LAXN2E','GF','n/a','https://eznoc.net2ez.com/customer/main/index/'),('Equinix','UKE','LD5EQI','GF','servicedesk.uk@eu.equinix.com ','none'),('Equinix','IND','SG2EQI','GF','ap-noc@ap.equinix.com','same as HKGEQI'),('Equinix','AUC','SY3EQI','GF','noc@ap.equinix.com','NONE NOTED'),('Equinix','HKG','TY2EQI','GF','tyo-support@ap.equinix.com','HKGEQI'),('Equinix','EEU','ZH2EQI','GF','servicedesk.zh@eu.equinix.com','AMSEQI');
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
) ENGINE=InnoDB AUTO_INCREMENT=15 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `external_sources`
--

LOCK TABLES `external_sources` WRITE;
/*!40000 ALTER TABLE `external_sources` DISABLE KEYS */;
INSERT INTO `external_sources` VALUES (1,'Telecity','dcp','updateme','updateme','updateme'),(2,'Equinix','dcp','','',''),(3,'Net2EZ','dcp','','',''),(4,'Terremark','dcp','786-547-8551','1-800-983-7060','updateme'),(5,'Telia Sonera','car','carrier-csc@teliasonera.com','(888) 436-1133','http://.telia.net/'),(6,'Telus','per','peering@telus.com','na','http://telus.com'),(7,'LimeLight Networks','car','support@limelightnetworks.com','proserv@limelightnetworks.com','http://llnw.net'),(8,'LINX','per','support@linx.net','+44 207 645 3535','http://linx.net'),(9,'InMotion Hosting','shc','advanced@inmotionhosting.com','support@inmotionhosting.com','http://inmotionhosting.com'),(10,'IX-REACH','per','support@ixreach.com','','http://ixreach.com'),(11,'Highwinds','car','noc@hwng.net','support@highwinds.com','http://highwinds.com'),(12,'Level 3','car','tscip@level3.com','(877) 453-8353','http://level3.com'),(13,'Brocade','man','support@brocade.com','http://my.brocade.com/wps/myportal/myhome','http://brocade.com'),(14,'Cologix','dcp','torontosupport@cologix.com','+1.416.846.1926 ','http://www.cologix.com/en/data-centers/toronto-col');
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
INSERT INTO `fam_corps` VALUES ('Data Foundry, Inc.','DF','Shane Menking','updateme'),('Giganews, Inc.','GN','Sunday Yokubaitis','updateme'),('Golden Frog','GF','Sunday Yokubaitis','updateme'),('Powerhouse Mgmt.','PH','Ron Yokubaitis','updateme'),('Texas.net','TN','Caroline Yokubaitis','updateme');
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
  `full_drivename` varchar(40) NOT NULL,
  `site` varchar(4) DEFAULT NULL,
  `host_shortname` varchar(25) NOT NULL,
  `drive_label` varchar(5) NOT NULL,
  `port` int(11) NOT NULL DEFAULT '0',
  `spool_number` int(11) DEFAULT NULL,
  `drive_status` varchar(9) DEFAULT NULL,
  `spool_status` varchar(11) DEFAULT NULL,
  `spool_fill` int(11) DEFAULT NULL,
  `pending_sectors` int(11) NOT NULL DEFAULT '0',
  `reallocated_sectors` int(11) NOT NULL DEFAULT '0',
  `raw_read_error_rate` int(11) NOT NULL DEFAULT '0',
  `in_use` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`full_drivename`),
  UNIQUE KEY `full_drivename` (`full_drivename`),
  KEY `host_shortname` (`host_shortname`),
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
  `rg_id` int(11) NOT NULL AUTO_INCREMENT,
  `rg_name` varchar(30) NOT NULL,
  `rg_code` varchar(3) NOT NULL,
  PRIMARY KEY (`rg_id`),
  UNIQUE KEY `rg_id` (`rg_id`),
  UNIQUE KEY `rg_code` (`rg_code`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gd_regions`
--

LOCK TABLES `gd_regions` WRITE;
/*!40000 ALTER TABLE `gd_regions` DISABLE KEYS */;
INSERT INTO `gd_regions` VALUES (1,'North America','NA'),(2,'Texas','TX'),(3,'Europe','EU'),(4,'East Asia','EA'),(5,'Oceania','AU'),(6,'Middle East','ME'),(7,'South America','SA'),(8,'Africa','AF'),(9,'Greater Russia','GR');
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
  KEY `asset_type` (`asset_type`),
  KEY `server_type` (`server_type`),
  KEY `site` (`site`),
  CONSTRAINT `gd_servers_ibfk_1` FOREIGN KEY (`asset_type`) REFERENCES `asset_cat` (`asset_type`),
  CONSTRAINT `gd_servers_ibfk_2` FOREIGN KEY (`server_type`) REFERENCES `server_cat` (`server_type`),
  CONSTRAINT `gd_servers_ibfk_3` FOREIGN KEY (`site`) REFERENCES `gd_sites` (`site_name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gd_servers`
--

LOCK TABLES `gd_servers` WRITE;
/*!40000 ALTER TABLE `gd_servers` DISABLE KEYS */;
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
  `srg_code` varchar(3) NOT NULL,
  `country` varchar(25) DEFAULT NULL,
  `dc_provider` varchar(6) NOT NULL,
  `rh_contact` varchar(50) DEFAULT NULL,
  `dc_address` varchar(50) DEFAULT NULL,
  `zip_code` varchar(7) DEFAULT NULL,
  PRIMARY KEY (`site_name`),
  UNIQUE KEY `site_name` (`site_name`),
  KEY `dc_provider` (`dc_provider`),
  KEY `srg_code` (`srg_code`),
  CONSTRAINT `gd_sites_ibfk_1` FOREIGN KEY (`dc_provider`) REFERENCES `dc_providers` (`pr_shortname`),
  CONSTRAINT `gd_sites_ibfk_2` FOREIGN KEY (`srg_code`) REFERENCES `gd_subregions` (`srg_code`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gd_sites`
--

LOCK TABLES `gd_sites` WRITE;
/*!40000 ALTER TABLE `gd_sites` DISABLE KEYS */;
INSERT INTO `gd_sites` VALUES ('AMS','AMS','Netherlands','AMSTEL','NL.support.SP@telecity.com','Suite R-A Kruislaan 411','1098 SJ'),('AMS2','AMS','Netherlands','AMSEQI','servicedesk.nl@eu.equinix.com','Luttenbergweg 4 ZUIDOOST','10101EC'),('AMS3','AMS','Netherlands','AMSTEL','NL.support.ABP@telecity.com','H.J.E. Wenckebachweg 127','1098 AM'),('DCA','DCA','United States','DCAEQI','https://ecc.equinix.com/sso/index.jsp','44470 Chilum Place, Bldg 1','20147'),('DCA1','DCA','United States','DCAEQI','request@datafoundry.com','21551 Beaumeade Circle','20147'),('DCA2','DCA','United States','DCAEQI','https://ecc.equinix.com/sso/index.jsp','21701 Filigree Court','20147'),('DCA3','DCA','United States','DCAN2E','https://eznoc.net2ez.com/customer/main/index/','44480 Hastings Drive, Ste 1000, Bldg 1','20147-6'),('HKG','HKG','China','HKGEQI','bx-hkg-support@ap.equinix.com','13/F Global Gateway 168 Yeung Uk Road','na'),('LAX','USW','United States','LAXN2E','https://eznoc.net2ez.com/customer/main/index/','600 W. 7th St, Ste 550','90017');
/*!40000 ALTER TABLE `gd_sites` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `gd_subregions`
--

DROP TABLE IF EXISTS `gd_subregions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `gd_subregions` (
  `srg_id` int(11) NOT NULL AUTO_INCREMENT,
  `srg_name` varchar(50) NOT NULL,
  `srg_code` varchar(3) NOT NULL,
  `srg_country` varchar(30) DEFAULT NULL,
  `rg_code` varchar(3) NOT NULL,
  PRIMARY KEY (`srg_name`),
  UNIQUE KEY `srg_name` (`srg_name`),
  UNIQUE KEY `srg_code` (`srg_code`),
  KEY `rg_code` (`rg_code`),
  CONSTRAINT `gd_subregions_ibfk_1` FOREIGN KEY (`rg_code`) REFERENCES `gd_regions` (`rg_code`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `gd_subregions`
--

LOCK TABLES `gd_subregions` WRITE;
/*!40000 ALTER TABLE `gd_subregions` DISABLE KEYS */;
INSERT INTO `gd_subregions` VALUES (3,'AMSTERDAM','AMS','NETHERLANDS','EU'),(11,'Arabian Puninsula','ARP','Various','ME'),(1,'Ashburn','DCA','UNITED STATES','NA'),(0,'AUSTIN','AUS','UNITED STATES','TX'),(8,'Australian Continent','AUC','Australia','AU'),(10,'Caspian Sea Area','CSP','Various','ME'),(5,'Eastern Europe','EEU','Various','EU'),(15,'Former Russian Satellites','FRS','Various','ME'),(4,'HONG KONG','HKG','CHINA','EA'),(7,'Indonesia','IND','Various','EA'),(2,'LAX','LAX','UNITED STATES','NA'),(9,'New Zealand','NZL','New Zealand','AU'),(6,'United Kingdom','UKE','United Kingdom','EU'),(13,'US/Canada East Coast','USE','Various','NA'),(14,'US/Canada West Coast','USW','Various','NA'),(12,'Western Europe','WEU','Various','ME');
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
  `type_desc` varchar(30) NOT NULL,
  PRIMARY KEY (`type_id`),
  UNIQUE KEY `type_id` (`type_id`),
  UNIQUE KEY `server_type` (`server_type`)
) ENGINE=InnoDB AUTO_INCREMENT=61 DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `server_cat`
--

LOCK TABLES `server_cat` WRITE;
/*!40000 ALTER TABLE `server_cat` DISABLE KEYS */;
INSERT INTO `server_cat` VALUES (22,'backlog', 'Backlog Servers'),(21,'border','Inter-Site/peer Border Transit Servers'),(23,'buffer', 'Article Buffers'),(2,'cruncher', 'Cruncher - graphing/logs'),(50,'cs', 'Console Server',),(0,'db', 'Database server'),(24,'gcf', 'Feeder Server'),(11,'h_server', 'Header Server'),(51,'kvm', 'Keyboard Virtual Machine'),(5,'lb', 'load balancer'),(27,'logstash', 'log repository'),(20,'num', 'Article Numbering Server'),(60,'other', 'Misc'),(10,'r_server', 'Binary (R)etentin Server'),(26,'search', 'NNTP indexing server'),(4,'serv', 'Site Service Machine'),(40,'s_server', '(S)torage Server (Dump Truck)'),(12,'t_server', '(T)ext Retention Server'),(30,'vmhost', 'Virtual Machine Host'),(32,'vpn', 'VPN Virtual Server'),(31,'vpnhost', 'VPN VM Host'),(25,'xref', 'Article Xref Server');
/*!40000 ALTER TABLE `server_cat` ENABLE KEYS */;
UNLOCK TABLES;

