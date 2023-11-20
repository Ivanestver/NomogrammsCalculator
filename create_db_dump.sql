CREATE DATABASE  IF NOT EXISTS `nomogramms_db` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `nomogramms_db`;
-- MySQL dump 10.13  Distrib 8.0.34, for Win64 (x86_64)
--
-- Host: localhost    Database: nomogramms_db
-- ------------------------------------------------------
-- Server version	8.0.35

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `measure`
--

DROP TABLE IF EXISTS `measure`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `measure` (
  `measure_id` varchar(100) NOT NULL,
  `measure_name` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`measure_id`),
  UNIQUE KEY `measure_id` (`measure_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `measure`
--

LOCK TABLES `measure` WRITE;
/*!40000 ALTER TABLE `measure` DISABLE KEYS */;
INSERT INTO `measure` VALUES ('1a9a2d16-694e-4736-a1f0-6c7a7f5faa8e','Масса'),('39a91a58-aac8-404f-aec6-ab7cff90bcf1','Скорость'),('985373dd-cd0e-4570-8318-2a969d2179d8','Высота');
/*!40000 ALTER TABLE `measure` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `measure_unit`
--

DROP TABLE IF EXISTS `measure_unit`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `measure_unit` (
  `measure_unit_id` varchar(100) NOT NULL,
  `measure_id` varchar(100) DEFAULT NULL,
  `unit_id` varchar(100) DEFAULT NULL,
  `measure_unit_name` varchar(1000) DEFAULT NULL,
  PRIMARY KEY (`measure_unit_id`),
  UNIQUE KEY `measure_unit_id` (`measure_unit_id`),
  KEY `measure_id` (`measure_id`),
  KEY `unit_id` (`unit_id`),
  CONSTRAINT `measure_unit_ibfk_1` FOREIGN KEY (`measure_id`) REFERENCES `measure` (`measure_id`),
  CONSTRAINT `measure_unit_ibfk_2` FOREIGN KEY (`unit_id`) REFERENCES `unit` (`unit_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `measure_unit`
--

LOCK TABLES `measure_unit` WRITE;
/*!40000 ALTER TABLE `measure_unit` DISABLE KEYS */;
INSERT INTO `measure_unit` VALUES ('4feb942c-ad0d-4919-b1dd-b13883803306','985373dd-cd0e-4570-8318-2a969d2179d8','e031c8a2-1175-4ff4-923c-da5a3386f6d8','Высота, м'),('7737a84f-0807-40d6-9e68-806e7e23768e','1a9a2d16-694e-4736-a1f0-6c7a7f5faa8e','7a5d44d0-765f-4263-bb0d-8385424e2006','Масса, кг'),('e44132a8-1e72-4c05-a7a5-4293c0e5e625','39a91a58-aac8-404f-aec6-ab7cff90bcf1','6a181dad-2019-4c04-9f36-1e5b8411b55c','Скорость, км');
/*!40000 ALTER TABLE `measure_unit` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `nets`
--

DROP TABLE IF EXISTS `nets`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `nets` (
  `net_id` varchar(100) NOT NULL,
  `net_name` varchar(100) NOT NULL DEFAULT '',
  `net_file` varchar(1000) NOT NULL DEFAULT '',
  PRIMARY KEY (`net_id`),
  UNIQUE KEY `net_id` (`net_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `nets`
--

LOCK TABLES `nets` WRITE;
/*!40000 ALTER TABLE `nets` DISABLE KEYS */;
/*!40000 ALTER TABLE `nets` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `property`
--

DROP TABLE IF EXISTS `property`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `property` (
  `property_id` varchar(100) NOT NULL,
  `property_name` varchar(1000) NOT NULL DEFAULT '',
  `property_value` varchar(100) NOT NULL DEFAULT '',
  PRIMARY KEY (`property_id`),
  UNIQUE KEY `property_id` (`property_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `property`
--

LOCK TABLES `property` WRITE;
/*!40000 ALTER TABLE `property` DISABLE KEYS */;
INSERT INTO `property` VALUES ('581060F1-2BD0-43AA-9C80-0E9107068890','nomogramm_bypassRule','Правило обхода'),('8D7A1E11-C43F-4C67-BADB-888AA3F334A9','dbobject_name','Название'),('8F8F8660-6AE1-4C79-8B96-0832903C5772','measure_value','Единица измерения'),('C3AA13ED-4620-4016-A106-7AAE653CBFD1','data_type_with_unit','Тип данных с единицей измерения'),('E67ABF23-9E60-49A4-BB5E-DC44F50B649E','data_output_type_with_unit','Тип выходных данных с единицей измерения'),('ED6368E7-897B-4951-9705-6E7B74FD359D','nomogramm_bypassRule_List','Список правил обхода номограмм');
/*!40000 ALTER TABLE `property` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `template`
--

DROP TABLE IF EXISTS `template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `template` (
  `template_id` varchar(100) NOT NULL,
  `class_id` varchar(100) NOT NULL,
  PRIMARY KEY (`template_id`),
  UNIQUE KEY `template_id` (`template_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `template`
--

LOCK TABLES `template` WRITE;
/*!40000 ALTER TABLE `template` DISABLE KEYS */;
INSERT INTO `template` VALUES ('87a78c3e-fce2-4a3c-9c00-819809621c02','f5313633-c8fc-43dc-a92e-88b7ee8df439'),('c5994fca-e751-40ca-9e79-67fbe46f0bbb','6dce13ae-8897-48ec-9b4e-664845d40d73'),('e79013aa-6b7d-42f2-9602-108907de3917','a8a4951d-8542-4cfa-b593-ecba3de727d1');
/*!40000 ALTER TABLE `template` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `template_measure_unit_input`
--

DROP TABLE IF EXISTS `template_measure_unit_input`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `template_measure_unit_input` (
  `template_id` varchar(100) DEFAULT NULL,
  `measure_unit_id` varchar(100) DEFAULT NULL,
  KEY `template_id` (`template_id`),
  KEY `measure_unit_id` (`measure_unit_id`),
  CONSTRAINT `template_measure_unit_input_ibfk_1` FOREIGN KEY (`template_id`) REFERENCES `template` (`template_id`),
  CONSTRAINT `template_measure_unit_input_ibfk_2` FOREIGN KEY (`measure_unit_id`) REFERENCES `measure_unit` (`measure_unit_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `template_measure_unit_input`
--

LOCK TABLES `template_measure_unit_input` WRITE;
/*!40000 ALTER TABLE `template_measure_unit_input` DISABLE KEYS */;
INSERT INTO `template_measure_unit_input` VALUES ('87a78c3e-fce2-4a3c-9c00-819809621c02','e44132a8-1e72-4c05-a7a5-4293c0e5e625'),('c5994fca-e751-40ca-9e79-67fbe46f0bbb','4feb942c-ad0d-4919-b1dd-b13883803306');
/*!40000 ALTER TABLE `template_measure_unit_input` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `template_measure_unit_output`
--

DROP TABLE IF EXISTS `template_measure_unit_output`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `template_measure_unit_output` (
  `template_id` varchar(100) DEFAULT NULL,
  `measure_unit_id` varchar(100) DEFAULT NULL,
  KEY `template_id` (`template_id`),
  KEY `measure_unit_id` (`measure_unit_id`),
  CONSTRAINT `template_measure_unit_output_ibfk_1` FOREIGN KEY (`template_id`) REFERENCES `template` (`template_id`),
  CONSTRAINT `template_measure_unit_output_ibfk_2` FOREIGN KEY (`measure_unit_id`) REFERENCES `measure_unit` (`measure_unit_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `template_measure_unit_output`
--

LOCK TABLES `template_measure_unit_output` WRITE;
/*!40000 ALTER TABLE `template_measure_unit_output` DISABLE KEYS */;
INSERT INTO `template_measure_unit_output` VALUES ('c5994fca-e751-40ca-9e79-67fbe46f0bbb','7737a84f-0807-40d6-9e68-806e7e23768e');
/*!40000 ALTER TABLE `template_measure_unit_output` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `template_property`
--

DROP TABLE IF EXISTS `template_property`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `template_property` (
  `template_id` varchar(100) NOT NULL,
  `property_id` varchar(100) NOT NULL,
  `property_value` varchar(1000) NOT NULL DEFAULT '',
  PRIMARY KEY (`template_id`,`property_id`),
  KEY `property_id` (`property_id`),
  CONSTRAINT `template_property_ibfk_1` FOREIGN KEY (`template_id`) REFERENCES `template` (`template_id`),
  CONSTRAINT `template_property_ibfk_2` FOREIGN KEY (`property_id`) REFERENCES `property` (`property_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `template_property`
--

LOCK TABLES `template_property` WRITE;
/*!40000 ALTER TABLE `template_property` DISABLE KEYS */;
INSERT INTO `template_property` VALUES ('87a78c3e-fce2-4a3c-9c00-819809621c02','581060f1-2bd0-43aa-9c80-0e9107068890',''),('87a78c3e-fce2-4a3c-9c00-819809621c02','8d7a1e11-c43f-4c67-badb-888aa3f334a9','test nomogramm'),('87a78c3e-fce2-4a3c-9c00-819809621c02','c3aa13ed-4620-4016-a106-7aae653cbfd1',''),('87a78c3e-fce2-4a3c-9c00-819809621c02','ed6368e7-897b-4951-9705-6e7b74fd359d',''),('c5994fca-e751-40ca-9e79-67fbe46f0bbb','8d7a1e11-c43f-4c67-badb-888aa3f334a9','test graphics'),('c5994fca-e751-40ca-9e79-67fbe46f0bbb','c3aa13ed-4620-4016-a106-7aae653cbfd1','test graphics'),('c5994fca-e751-40ca-9e79-67fbe46f0bbb','e67abf23-9e60-49a4-bb5e-dc44f50b649e','test graphics'),('e79013aa-6b7d-42f2-9602-108907de3917','8d7a1e11-c43f-4c67-badb-888aa3f334a9','test');
/*!40000 ALTER TABLE `template_property` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `template_template`
--

DROP TABLE IF EXISTS `template_template`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `template_template` (
  `master_id` varchar(100) NOT NULL,
  `sub_id` varchar(100) NOT NULL,
  `extra` varchar(1000) NOT NULL DEFAULT '',
  PRIMARY KEY (`master_id`,`sub_id`),
  KEY `sub_id` (`sub_id`),
  CONSTRAINT `template_template_ibfk_1` FOREIGN KEY (`master_id`) REFERENCES `template` (`template_id`),
  CONSTRAINT `template_template_ibfk_2` FOREIGN KEY (`sub_id`) REFERENCES `template` (`template_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `template_template`
--

LOCK TABLES `template_template` WRITE;
/*!40000 ALTER TABLE `template_template` DISABLE KEYS */;
INSERT INTO `template_template` VALUES ('87a78c3e-fce2-4a3c-9c00-819809621c02','c5994fca-e751-40ca-9e79-67fbe46f0bbb',''),('e79013aa-6b7d-42f2-9602-108907de3917','87a78c3e-fce2-4a3c-9c00-819809621c02','');
/*!40000 ALTER TABLE `template_template` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `unit`
--

DROP TABLE IF EXISTS `unit`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `unit` (
  `unit_id` varchar(100) NOT NULL,
  `unit_name` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`unit_id`),
  UNIQUE KEY `unit_id` (`unit_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `unit`
--

LOCK TABLES `unit` WRITE;
/*!40000 ALTER TABLE `unit` DISABLE KEYS */;
INSERT INTO `unit` VALUES ('6a181dad-2019-4c04-9f36-1e5b8411b55c','Км'),('7a5d44d0-765f-4263-bb0d-8385424e2006','Кг'),('e031c8a2-1175-4ff4-923c-da5a3386f6d8','М');
/*!40000 ALTER TABLE `unit` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2023-11-20 21:21:55
