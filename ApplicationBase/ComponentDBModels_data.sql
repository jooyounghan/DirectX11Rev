-- MySQL dump 10.13  Distrib 8.0.40, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: component_db
-- ------------------------------------------------------
-- Server version	8.0.40

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
-- Table structure for table `camera_components`
--

DROP TABLE IF EXISTS `camera_components`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `camera_components` (
  `camera_component_id` int unsigned NOT NULL,
  `width` smallint unsigned NOT NULL,
  `height` smallint unsigned NOT NULL,
  `near_z` float NOT NULL,
  `far_z` float NOT NULL,
  `fov_angle` float NOT NULL,
  PRIMARY KEY (`camera_component_id`),
  CONSTRAINT `component_id_fk6` FOREIGN KEY (`camera_component_id`) REFERENCES `components` (`component_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `camera_components`
--

LOCK TABLES `camera_components` WRITE;
/*!40000 ALTER TABLE `camera_components` DISABLE KEYS */;
/*!40000 ALTER TABLE `camera_components` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `collision_options`
--

DROP TABLE IF EXISTS `collision_options`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `collision_options` (
  `collision_option_id` int unsigned NOT NULL AUTO_INCREMENT,
  `collision_option_description` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_520_ci NOT NULL,
  PRIMARY KEY (`collision_option_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `collision_options`
--

LOCK TABLES `collision_options` WRITE;
/*!40000 ALTER TABLE `collision_options` DISABLE KEYS */;
INSERT INTO `collision_options` VALUES (1,'render_control');
/*!40000 ALTER TABLE `collision_options` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `component_types`
--

DROP TABLE IF EXISTS `component_types`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `component_types` (
  `component_type` int unsigned NOT NULL AUTO_INCREMENT,
  `component_type_description` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  PRIMARY KEY (`component_type`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `component_types`
--

LOCK TABLES `component_types` WRITE;
/*!40000 ALTER TABLE `component_types` DISABLE KEYS */;
INSERT INTO `component_types` VALUES (1,'static_mesh'),(2,'skeletal_mesh'),(3,'camera'),(4,'sphere_collision'),(5,'oriented_box_collision');
/*!40000 ALTER TABLE `component_types` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `components`
--

DROP TABLE IF EXISTS `components`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `components` (
  `component_id` int unsigned NOT NULL AUTO_INCREMENT,
  `parent_component_id` int unsigned NOT NULL DEFAULT '0',
  `component_name` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_520_ci NOT NULL DEFAULT 'NULL',
  `position_x` float NOT NULL DEFAULT '0',
  `position_y` float NOT NULL DEFAULT '0',
  `position_z` float NOT NULL DEFAULT '0',
  `angle_x` float NOT NULL DEFAULT '0',
  `angle_y` float NOT NULL DEFAULT '0',
  `angle_z` float NOT NULL DEFAULT '0',
  `scale_x` float NOT NULL DEFAULT '0',
  `scale_y` float NOT NULL DEFAULT '0',
  `scale_z` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`component_id`)
) ENGINE=InnoDB AUTO_INCREMENT=13 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `components`
--

LOCK TABLES `components` WRITE;
/*!40000 ALTER TABLE `components` DISABLE KEYS */;
INSERT INTO `components` VALUES (1,0,'SphereCollision1',-982,-729,0,0,0,0,1,1,1),(2,0,'OrientedCollision1',-750,0,0,-1.6,-1.7,0,1,1,1),(3,1,'Skeletal1',0,-101,0,0,0,0,1,1,1),(4,2,'Skeletal2',0,-88,0,0,-5.8,0,1,1,1),(5,0,'SphereCollision2',393,-846,0,0,0,0,1,1,1),(6,0,'SphereCollision3',-223,57,0,0,0,0,1,1,1),(7,0,'OrientedCollision2',750,0,0,0,0,0,1,1,1),(8,0,'OrientedCollision3',1250,0,0,0,-23,0,1,1,1),(9,5,'Skeletal3',0,-88,0,0,0,0,1,1,1),(10,6,'Skeleta4',0,-85,0,0,0,0,1,1,1),(11,7,'Skeletal5',0,-88,0,0,0,0,1,1,1),(12,8,'Skeletal6',0,-88,0,0,0,0,1,1,1);
/*!40000 ALTER TABLE `components` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `components_to_type`
--

DROP TABLE IF EXISTS `components_to_type`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `components_to_type` (
  `component_id` int unsigned NOT NULL,
  `component_type` int unsigned NOT NULL,
  PRIMARY KEY (`component_id`),
  KEY `components_to_type_comopnent_type_idx` (`component_type`),
  CONSTRAINT `component_id_fk1` FOREIGN KEY (`component_id`) REFERENCES `components` (`component_id`),
  CONSTRAINT `component_type_fk1` FOREIGN KEY (`component_type`) REFERENCES `component_types` (`component_type`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `components_to_type`
--

LOCK TABLES `components_to_type` WRITE;
/*!40000 ALTER TABLE `components_to_type` DISABLE KEYS */;
INSERT INTO `components_to_type` VALUES (3,2),(4,2),(9,2),(10,2),(11,2),(12,2),(1,4),(5,4),(6,4),(2,5),(7,5),(8,5);
/*!40000 ALTER TABLE `components_to_type` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `mesh_component_informations`
--

DROP TABLE IF EXISTS `mesh_component_informations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `mesh_component_informations` (
  `mesh_component_id` int unsigned NOT NULL,
  `material_names` json DEFAULT NULL,
  PRIMARY KEY (`mesh_component_id`),
  KEY `mesh_component_id_idx` (`mesh_component_id`),
  CONSTRAINT `component_id_fk3` FOREIGN KEY (`mesh_component_id`) REFERENCES `components` (`component_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `mesh_component_informations`
--

LOCK TABLES `mesh_component_informations` WRITE;
/*!40000 ALTER TABLE `mesh_component_informations` DISABLE KEYS */;
INSERT INTO `mesh_component_informations` VALUES (3,'{\"material_names\": [\"VanguardBodyMat\", \"VanguardBodyMat\"]}'),(4,'{\"material_names\": [\"VanguardBodyMat\", \"VanguardBodyMat\"]}'),(9,'{\"material_names\": [\"VanguardBodyMat\", \"VanguardBodyMat\"]}'),(10,'{\"material_names\": [\"VanguardBodyMat\", \"VanguardBodyMat\"]}'),(11,'{\"material_names\": [\"VanguardBodyMat\", \"VanguardBodyMat\"]}'),(12,'{\"material_names\": [\"VanguardBodyMat\", \"VanguardBodyMat\"]}');
/*!40000 ALTER TABLE `mesh_component_informations` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `oriented_box_collision_components`
--

DROP TABLE IF EXISTS `oriented_box_collision_components`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `oriented_box_collision_components` (
  `oriented_box_collision_component_id` int unsigned NOT NULL AUTO_INCREMENT,
  `extent_x` float NOT NULL,
  `extent_y` float NOT NULL,
  `extent_z` float NOT NULL,
  `collision_option_id` int unsigned NOT NULL,
  PRIMARY KEY (`oriented_box_collision_component_id`),
  KEY `collision_option_fk2_idx` (`collision_option_id`),
  CONSTRAINT `collision_option_fk2` FOREIGN KEY (`collision_option_id`) REFERENCES `collision_options` (`collision_option_id`) ON UPDATE CASCADE,
  CONSTRAINT `component_id_fk8` FOREIGN KEY (`oriented_box_collision_component_id`) REFERENCES `components` (`component_id`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `oriented_box_collision_components`
--

LOCK TABLES `oriented_box_collision_components` WRITE;
/*!40000 ALTER TABLE `oriented_box_collision_components` DISABLE KEYS */;
INSERT INTO `oriented_box_collision_components` VALUES (2,100,200,100,1),(7,81.9,200,50,1),(8,80,200,80,1);
/*!40000 ALTER TABLE `oriented_box_collision_components` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `scene_informations`
--

DROP TABLE IF EXISTS `scene_informations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `scene_informations` (
  `scene_information_id` int unsigned NOT NULL AUTO_INCREMENT,
  `scene_id` int unsigned NOT NULL,
  `component_id` int unsigned NOT NULL,
  PRIMARY KEY (`scene_information_id`),
  KEY `scene_id_idx` (`scene_id`),
  KEY `scene_component_id_idx` (`component_id`),
  CONSTRAINT `component_id_fk2` FOREIGN KEY (`component_id`) REFERENCES `components` (`component_id`) ON UPDATE CASCADE,
  CONSTRAINT `scene_id_fk1` FOREIGN KEY (`scene_id`) REFERENCES `scenes` (`scene_id`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=11 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `scene_informations`
--

LOCK TABLES `scene_informations` WRITE;
/*!40000 ALTER TABLE `scene_informations` DISABLE KEYS */;
INSERT INTO `scene_informations` VALUES (1,1,1),(2,1,2),(3,1,5),(4,1,6),(5,1,7),(6,1,8);
/*!40000 ALTER TABLE `scene_informations` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `scenes`
--

DROP TABLE IF EXISTS `scenes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `scenes` (
  `scene_id` int unsigned NOT NULL AUTO_INCREMENT,
  `scene_description` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `scene_static_mesh_name` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_520_ci NOT NULL,
  `scene_ibl_material_name` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_520_ci NOT NULL,
  PRIMARY KEY (`scene_id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `scenes`
--

LOCK TABLES `scenes` WRITE;
/*!40000 ALTER TABLE `scenes` DISABLE KEYS */;
INSERT INTO `scenes` VALUES (1,'test_map','sphere_scene_Static','EveningSky');
/*!40000 ALTER TABLE `scenes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `skeletal_mesh_components`
--

DROP TABLE IF EXISTS `skeletal_mesh_components`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `skeletal_mesh_components` (
  `skeletal_mesh_component_id` int unsigned NOT NULL,
  `skeletal_mesh_name` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_520_ci DEFAULT NULL,
  PRIMARY KEY (`skeletal_mesh_component_id`),
  CONSTRAINT `component_id_fk4` FOREIGN KEY (`skeletal_mesh_component_id`) REFERENCES `mesh_component_informations` (`mesh_component_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `skeletal_mesh_components`
--

LOCK TABLES `skeletal_mesh_components` WRITE;
/*!40000 ALTER TABLE `skeletal_mesh_components` DISABLE KEYS */;
INSERT INTO `skeletal_mesh_components` VALUES (3,'Vanguard_LOD_Skeletal'),(4,'Vanguard_LOD_Skeletal'),(9,'Vanguard_LOD_Skeletal'),(10,'Vanguard_LOD_Skeletal'),(11,'Vanguard_LOD_Skeletal'),(12,'Vanguard_LOD_Skeletal');
/*!40000 ALTER TABLE `skeletal_mesh_components` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sphere_collision_components`
--

DROP TABLE IF EXISTS `sphere_collision_components`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `sphere_collision_components` (
  `sphere_collision_component_id` int unsigned NOT NULL AUTO_INCREMENT,
  `radius` float NOT NULL,
  `collision_option_id` int unsigned NOT NULL,
  PRIMARY KEY (`sphere_collision_component_id`),
  KEY `collision_option_fk1_idx` (`collision_option_id`),
  CONSTRAINT `collision_option_fk1` FOREIGN KEY (`collision_option_id`) REFERENCES `collision_options` (`collision_option_id`) ON UPDATE CASCADE,
  CONSTRAINT `component_id_fk7` FOREIGN KEY (`sphere_collision_component_id`) REFERENCES `components` (`component_id`) ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sphere_collision_components`
--

LOCK TABLES `sphere_collision_components` WRITE;
/*!40000 ALTER TABLE `sphere_collision_components` DISABLE KEYS */;
INSERT INTO `sphere_collision_components` VALUES (1,100,1),(5,150,1),(6,200,1);
/*!40000 ALTER TABLE `sphere_collision_components` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `static_mesh_components`
--

DROP TABLE IF EXISTS `static_mesh_components`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `static_mesh_components` (
  `static_mesh_component_id` int unsigned NOT NULL,
  `static_mesh_name` varchar(45) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_520_ci DEFAULT NULL,
  PRIMARY KEY (`static_mesh_component_id`),
  CONSTRAINT `component_id_fk5` FOREIGN KEY (`static_mesh_component_id`) REFERENCES `mesh_component_informations` (`mesh_component_id`) ON UPDATE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_520_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `static_mesh_components`
--

LOCK TABLES `static_mesh_components` WRITE;
/*!40000 ALTER TABLE `static_mesh_components` DISABLE KEYS */;
/*!40000 ALTER TABLE `static_mesh_components` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-01-31 22:35:17
