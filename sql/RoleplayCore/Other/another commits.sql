DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dh_demon_muzzle';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(389860, 'spell_dh_demon_muzzle'),
(204598, 'spell_dh_demon_muzzle'),
(207685, 'spell_dh_demon_muzzle'),
(204490, 'spell_dh_demon_muzzle'),
(204834, 'spell_dh_demon_muzzle');

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_dru_guardian_of_elune_healing');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(22842, 'spell_dru_guardian_of_elune_healing');

DELETE FROM `spell_proc` WHERE `SpellId` IN (155578);
INSERT INTO `spell_proc` (`SpellId`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`SpellFamilyMask3`,`ProcFlags`,`ProcFlags2`,`SpellTypeMask`,`SpellPhaseMask`,`HitMask`,`AttributesMask`,`DisableEffectsMask`,`ProcsPerMinute`,`Chance`,`Cooldown`,`Charges`) VALUES
(155578,0x00,7,0x00000000,0x00000040,0x00000000,0x00000000,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0,0,0,0); -- Guardian of Elune

DELETE FROM `spell_proc` WHERE `SpellId` IN (213680);
INSERT INTO `spell_proc` (`SpellId`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`SpellFamilyMask3`,`ProcFlags`,`ProcFlags2`,`SpellTypeMask`,`SpellPhaseMask`,`HitMask`,`AttributesMask`,`DisableEffectsMask`,`ProcsPerMinute`,`Chance`,`Cooldown`,`Charges`) VALUES
(213680,0x00,7,0x00000000,0x40000000,0x00004000,0x00000000,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0,0,0,0); -- Guardian of Elune

DELETE FROM `spell_proc` WHERE `SpellId` IN (383115);
INSERT INTO `spell_proc` (`SpellId`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`SpellFamilyMask3`,`ProcFlags`,`ProcFlags2`,`SpellTypeMask`,`SpellPhaseMask`,`HitMask`,`AttributesMask`,`DisableEffectsMask`,`ProcsPerMinute`,`Chance`,`Cooldown`,`Charges`) VALUES
(383115,0x00,4,0x00000000,0x00000000,0x00000000,0x00000000,0x0,0x0,0x0,0x2,0x1000,0x0,0x0,0,0,0,0); -- Concussive Blows

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warr_warlords_torment';
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(390140,'spell_warr_warlords_torment');

DELETE FROM `spell_proc` WHERE `SpellId` IN (390140);
INSERT INTO `spell_proc` (`SpellId`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`SpellFamilyMask3`,`ProcFlags`,`ProcFlags2`,`SpellTypeMask`,`SpellPhaseMask`,`HitMask`,`AttributesMask`,`DisableEffectsMask`,`ProcsPerMinute`,`Chance`,`Cooldown`,`Charges`) VALUES
(390140,0x00,4,0x00000000,0x00000000,0x00000000,0x00000000,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0,0,0,0); -- Warlord's Torment

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warr_bladesmasters_torment';
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(390138,'spell_warr_bladesmasters_torment');

DELETE FROM `spell_proc` WHERE `SpellId` IN (390138);
INSERT INTO `spell_proc` (`SpellId`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`SpellFamilyMask3`,`ProcFlags`,`ProcFlags2`,`SpellTypeMask`,`SpellPhaseMask`,`HitMask`,`AttributesMask`,`DisableEffectsMask`,`ProcsPerMinute`,`Chance`,`Cooldown`,`Charges`) VALUES
(390138,0x00,4,0x00000000,0x00000000,0x00000000,0x00000000,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0,0,0,0); -- Blademaster's Torment

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_dru_thorns_of_iron_damage');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(400223, 'spell_dru_thorns_of_iron_damage');

DELETE FROM `spell_proc` WHERE `SpellId` IN (400222);
INSERT INTO `spell_proc` (`SpellId`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`SpellFamilyMask3`,`ProcFlags`,`ProcFlags2`,`SpellTypeMask`,`SpellPhaseMask`,`HitMask`,`AttributesMask`,`DisableEffectsMask`,`ProcsPerMinute`,`Chance`,`Cooldown`,`Charges`) VALUES
(400222,0x00,7,0x00000000,0x00000000,0x00000000,0x00000000,0x0,0x0,0x0,0x4,0x0,0x0,0x0,0,0,0,0); -- Thorns of Iron

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_warr_intervene');
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(3411,'spell_warr_intervene'),
(316531,'spell_warr_intervene_charge');

DELETE FROM `spell_proc` WHERE `SpellId` IN (147833);
INSERT INTO `spell_proc` (`SpellId`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`SpellFamilyMask3`,`ProcFlags`,`ProcFlags2`,`SpellTypeMask`,`SpellPhaseMask`,`HitMask`,`AttributesMask`,`DisableEffectsMask`,`ProcsPerMinute`,`Chance`,`Cooldown`,`Charges`) VALUES
(147833,0x00,4,0x00000000,0x00000000,0x00000000,0x00000000,0x0,0x0,0x0,0x2,0x0,0x2,0x0,0,0,0,0); -- Intervene

INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(382551, 'spell_warr_pain_and_gain_heal');

DELETE FROM `spell_proc` WHERE `SpellId` IN (382549);
INSERT INTO `spell_proc` (`SpellId`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`SpellFamilyMask3`,`ProcFlags`,`ProcFlags2`,`SpellTypeMask`,`SpellPhaseMask`,`HitMask`,`AttributesMask`,`DisableEffectsMask`,`ProcsPerMinute`,`Chance`,`Cooldown`,`Charges`) VALUES
(382549,0x00,4,0x00000000,0x00000000,0x00000000,0x00000000,0x0,0x0,0x2,0x2,0x0,0x2,0x0,0,0,0,0); -- Pain and Gain

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_dru_elunes_favored', 'spell_dru_elunes_favored_proc');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(370586, 'spell_dru_elunes_favored'),
(370588, 'spell_dru_elunes_favored_proc');

DELETE FROM `spell_proc` WHERE `SpellId` IN (370588);
INSERT INTO `spell_proc` (`SpellId`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`SpellFamilyMask3`,`ProcFlags`,`ProcFlags2`,`SpellTypeMask`,`SpellPhaseMask`,`HitMask`,`AttributesMask`,`DisableEffectsMask`,`ProcsPerMinute`,`Chance`,`Cooldown`,`Charges`) VALUES
(370588,0x40,7,0x00000000,0x00000000,0x00000000,0x00000000,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0,0,0,0); -- Elune's Favored

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_dru_flower_walk', 'spell_dru_flower_walk_heal');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(22812, 'spell_dru_flower_walk'),
(439902, 'spell_dru_flower_walk_heal');

DELETE FROM `spell_proc` WHERE `SpellId` IN (395446);
INSERT INTO `spell_proc` (`SpellId`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`SpellFamilyMask3`,`ProcFlags`,`ProcFlags2`,`SpellTypeMask`,`SpellPhaseMask`,`HitMask`,`AttributesMask`,`DisableEffectsMask`,`ProcsPerMinute`,`Chance`,`Cooldown`,`Charges`) VALUES
(395446,0x00,107,0x00000000,0x00000002,0x00000000,0x00000000,0x0,0x0,0x5,0x2,0x0,0x0,0x0,0,0,0,0); -- Soul Sigils

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dh_soul_sigils';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(395446, 'spell_dh_soul_sigils');

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_dru_twin_moonfire', 'spell_dru_twin_moons_effect');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(8921, 'spell_dru_twin_moonfire'),
(281847, 'spell_dru_twin_moons_effect');

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warr_thunder_clap';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(6343, 'spell_warr_thunder_clap');

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warr_thunder_clap_rend';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(6343, 'spell_warr_thunder_clap_rend');

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dh_enduring_torment';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(452410, 'spell_dh_enduring_torment');

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_dru_moonless_night');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(400278, 'spell_dru_moonless_night');

DELETE FROM `spell_proc` WHERE `SpellId` IN (400278);
INSERT INTO `spell_proc` (`SpellId`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`SpellFamilyMask3`,`ProcFlags`,`ProcFlags2`,`SpellTypeMask`,`SpellPhaseMask`,`HitMask`,`AttributesMask`,`DisableEffectsMask`,`ProcsPerMinute`,`Chance`,`Cooldown`,`Charges`) VALUES
(400278,0x00,7,0x00000000,0x00000000,0x00000000,0x00000000,0x0,0x0,0x0,0x2,0x0,0x0,0x0,0,0,0,0); -- Moonless Night

DELETE FROM `spell_proc` WHERE `SpellId` IN (7384);
INSERT INTO `spell_proc` (`SpellId`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`SpellFamilyMask3`,`ProcFlags`,`ProcFlags2`,`SpellTypeMask`,`SpellPhaseMask`,`HitMask`,`AttributesMask`,`DisableEffectsMask`,`ProcsPerMinute`,`Chance`,`Cooldown`,`Charges`) VALUES
(7384,0x00,4,0x00000000,0x00000000,0x00000000,0x00000000,0x0,0x0,0x0,0x2,0x0,0x8,0x0,0,0,0,1); -- Overpower

DELETE FROM `spell_proc` WHERE `SpellId` IN (392792);
INSERT INTO `spell_proc` (`SpellId`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`SpellFamilyMask3`,`ProcFlags`,`ProcFlags2`,`SpellTypeMask`,`SpellPhaseMask`,`HitMask`,`AttributesMask`,`DisableEffectsMask`,`ProcsPerMinute`,`Chance`,`Cooldown`,`Charges`) VALUES
(392792,0x00,4,0x00000000,0x00000000,0x00000000,0x00000000,0x0,0x0,0x0,0x2,0x0,0x8,0x0,0,0,0,0); -- Frothing Berserker

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warr_frothing_berserker';
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(392792,'spell_warr_frothing_berserker');

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_dru_pulverize', 'spell_dru_pulverize_thrash');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(80313, 'spell_dru_pulverize'),
(77758, 'spell_dru_pulverize_thrash');