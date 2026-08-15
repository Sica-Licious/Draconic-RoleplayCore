-- Club Finder tables for guild recruitment system

DROP TABLE IF EXISTS `club_finder_applicant`;
DROP TABLE IF EXISTS `club_finder_post`;

CREATE TABLE `club_finder_post` (
    `postingID`         BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
    `guildID`           BIGINT UNSIGNED NOT NULL DEFAULT 0,
    `description`       TEXT            NOT NULL,
    `playstyle`         INT UNSIGNED    NOT NULL DEFAULT 0,
    `interests`         INT UNSIGNED    NOT NULL DEFAULT 0,
    `specIDs`           VARCHAR(256)    NOT NULL DEFAULT '' COMMENT 'Comma-separated spec IDs being recruited',
    `classMask`         INT UNSIGNED    NOT NULL DEFAULT 0,
    `minLevel`          TINYINT UNSIGNED NOT NULL DEFAULT 10,
    `maxLevel`          TINYINT UNSIGNED NOT NULL DEFAULT 80,
    `slotsAvailable`    INT UNSIGNED    NOT NULL DEFAULT 0,
    `maxApplicants`     INT UNSIGNED    NOT NULL DEFAULT 0xFFFFFFFF,
    `language`          INT UNSIGNED    NOT NULL DEFAULT 0,
    `status`            TINYINT UNSIGNED NOT NULL DEFAULT 0 COMMENT '0=Active, 1=Closed',
    `timestamp`         INT UNSIGNED    NOT NULL DEFAULT 0,
    PRIMARY KEY (`postingID`),
    KEY `idx_guildID` (`guildID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

CREATE TABLE `club_finder_applicant` (
    `id`                BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
    `postingID`         BIGINT UNSIGNED NOT NULL,
    `playerGUID`        BIGINT UNSIGNED NOT NULL,
    `status`            TINYINT UNSIGNED NOT NULL DEFAULT 0 COMMENT '0=Pending, 1=Accepted, 2=Declined',
    `comment`           TEXT            NOT NULL,
    `timestamp`         INT UNSIGNED    NOT NULL DEFAULT 0,
    PRIMARY KEY (`id`),
    KEY `idx_postingID` (`postingID`),
    KEY `idx_playerGUID` (`playerGUID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;
