USE classiccharacters;

DROP TABLE IF EXISTS custom_real_pvprank;
CREATE TABLE custom_real_pvprank (
    rank_id                 TINYINT UNSIGNED NOT NULL PRIMARY KEY,
    min_honor               INT UNSIGNED     NOT NULL,
    min_level               TINYINT UNSIGNED NOT NULL,
    title_alliance          VARCHAR(40),
    title_horde             VARCHAR(40),
    max_honor_this_rank_cap INT UNSIGNED     NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;

INSERT INTO custom_real_pvprank VALUES
(1,    100,  1, 'Private',            'Scout',               1999),
(2,   2000,  1, 'Corporal',           'Grunt',               4999),
(3,   5000,  1, 'Sergeant',           'Sergeant',            9999),
(4,  10000, 33, 'Master Sergeant',    'Senior Sergeant',    14999),
(5,  15000, 38, 'Sergeant Major',     'First Sergeant',     19999),
(6,  20000, 41, 'Knight',             'Stone Guard',        24999),
(7,  25000, 44, 'Knight-Lieutenant',  'Blood Guard',        29999),
(8,  30000, 46, 'Knight-Captain',     'Legionnaire',        34999),
(9,  35000, 48, 'Knight-Champion',    'Centurion',          39999),
(10, 40000, 51, 'Lieutenant Commander','Champion',          44999),
(11, 45000, 53, 'Commander',          'Lieutenant General', 49999),
(12, 50000, 55, 'Marshal',            'General',            54999),
(13, 55000, 57, 'Field Marshal',      'Warlord',            59999),
(14, 60000, 59, 'Grand Marshal',      'High Warlord',       75000);

DROP TABLE IF EXISTS custom_pvp_cap_last_run;
CREATE TABLE custom_pvp_cap_last_run (
    id       TINYINT PRIMARY KEY DEFAULT 1,
    last_run DATETIME NOT NULL DEFAULT '1970-01-01 00:00:01'
) ENGINE=InnoDB;

INSERT IGNORE INTO custom_pvp_cap_last_run (id) VALUES (1);

DELIMITER $$

DROP PROCEDURE IF EXISTS ApplyPvPRankLevelCap_Weekly $$
CREATE PROCEDURE ApplyPvPRankLevelCap_Weekly()
BEGIN
    DECLARE last_run_date DATE;
    DECLARE today_date DATE DEFAULT CURDATE();
    DECLARE last_wednesday DATE;

    SELECT DATE(last_run) INTO last_run_date FROM custom_pvp_cap_last_run WHERE id = 1;

    SET last_wednesday = today_date - INTERVAL ((WEEKDAY(today_date) + 4) % 7) DAY;

    IF last_run_date IS NULL OR last_run_date < last_wednesday THEN

        UPDATE characters AS c
        JOIN (
            SELECT c2.guid,
                   MAX(cr.max_honor_this_rank_cap) AS max_cap
            FROM characters AS c2
            JOIN custom_real_pvprank AS cr
              ON cr.min_level <= c2.level
            GROUP BY c2.guid
        ) AS caps ON caps.guid = c.guid
        SET c.stored_honor_rating = caps.max_cap
        WHERE c.stored_honor_rating > caps.max_cap;

        UPDATE custom_pvp_cap_last_run
        SET last_run = NOW()
        WHERE id = 1;

    END IF;

END $$

DELIMITER ;
