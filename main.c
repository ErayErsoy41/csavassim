#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct definitions for units, heroes, creatures, and research
struct Unit {
    char name[50];
    int count;
    int attack;
    int defense;
    int health;
    float crit_chance;
};

struct Hero {
    char name[50];
    float attack_bonus;
    float defense_bonus;
};

struct Creature {
    char name[50];
    float bonus_friendly;
    float penalty_enemy;
};

struct Research {
    int attack_bonus;
    int defense_bonus;
};

// Parse function to load data from multiple JSON-like inputs
void parseJSON(const char* faction, const char* file_content, struct Unit units[], int *unit_count,
               struct Hero heroes[], int *hero_count, struct Creature creatures[], int *creature_count,
               struct Research *research) {

    char line[256], unit_name[50], hero_name[50], creature_name[50], research_type[50];
    int count, level;

    const char* ptr = file_content;

    while (sscanf(ptr, " %255[^\n]\n", line) == 1) {
        // Check for unit parsing
        if (strstr(line, "piyadeler") || strstr(line, "suvariler") || strstr(line, "okcular") || strstr(line, "kusatma_makineleri") ||
            strstr(line, "ork_dovusculeri") || strstr(line, "mizrakcilar") || strstr(line, "varg_binicileri") || strstr(line, "troller")) {

            if (sscanf(line, "\"%[^\"]\" : %d,", unit_name, &count) == 2) {
                // Add unit based on its name
                strcpy(units[*unit_count].name, unit_name);
                units[*unit_count].count = count;

                if (strcmp(unit_name, "piyadeler") == 0) {
                    units[*unit_count].attack = 30;
                    units[*unit_count].defense = 40;
                    units[*unit_count].health = 100;
                    units[*unit_count].crit_chance = 0.05;
                } else if (strcmp(unit_name, "okcular") == 0) {
                    units[*unit_count].attack = 40;
                    units[*unit_count].defense = 20;
                    units[*unit_count].health = 80;
                    units[*unit_count].crit_chance = 0.10;
                } else if (strcmp(unit_name, "suvariler") == 0) {
                    units[*unit_count].attack = 50;
                    units[*unit_count].defense = 30;
                    units[*unit_count].health = 120;
                    units[*unit_count].crit_chance = 0.07;
                } else if (strcmp(unit_name, "kusatma_makineleri") == 0) {
                    units[*unit_count].attack = 100;
                    units[*unit_count].defense = 50;
                    units[*unit_count].health = 150;
                    units[*unit_count].crit_chance = 0.0;
                } else if (strcmp(unit_name, "ork_dovusculeri") == 0) {
                    units[*unit_count].attack = 25;
                    units[*unit_count].defense = 20;
                    units[*unit_count].health = 100;
                    units[*unit_count].crit_chance = 0.08;
                } else if (strcmp(unit_name, "mizrakcilar") == 0) {
                    units[*unit_count].attack = 30;
                    units[*unit_count].defense = 25;
                    units[*unit_count].health = 90;
                    units[*unit_count].crit_chance = 0.05;
                } else if (strcmp(unit_name, "varg_binicileri") == 0) {
                    units[*unit_count].attack = 40;
                    units[*unit_count].defense = 35;
                    units[*unit_count].health = 130;
                    units[*unit_count].crit_chance = 0.06;
                } else if (strcmp(unit_name, "troller") == 0) {
                    units[*unit_count].attack = 70;
                    units[*unit_count].defense = 40;
                    units[*unit_count].health = 200;
                    units[*unit_count].crit_chance = 0.05;
                }

                printf("Parsed unit: %s, Count: %d\n", unit_name, count);  // Debug output for unit parsing
                (*unit_count)++;
            }
        }

        // Check for hero parsing
        else if (strstr(line, "kahramanlar")) {
            if (sscanf(line, "\"kahramanlar\" : [ \"%[^\"]", hero_name) == 1) {
                strcpy(heroes[*hero_count].name, hero_name);
                heroes[*hero_count].attack_bonus = 0.15;  // Default bonus (adjust as necessary)
                heroes[*hero_count].defense_bonus = 0.10;
                printf("Parsed hero: %s\n", hero_name);  // Debug output for hero parsing
                (*hero_count)++;
            }
        }

        // Check for creature parsing
        else if (strstr(line, "canavarlar")) {
            if (sscanf(line, "\"canavarlar\" : [ \"%[^\"]", creature_name) == 1) {
                strcpy(creatures[*creature_count].name, creature_name);
                creatures[*creature_count].bonus_friendly = 0.15;  // Default bonus (adjust as necessary)
                creatures[*creature_count].penalty_enemy = 0.0;
                printf("Parsed creature: %s\n", creature_name);  // Debug output for creature parsing
                (*creature_count)++;
            }
        }

        // Check for research parsing
        else if (strstr(line, "savunma_ustaligi") || strstr(line, "saldiri_gelistirmesi")) {
            if (sscanf(line, "\"%[^\"]\" : %d", research_type, &level) == 2) {
                if (strcmp(research_type, "savunma_ustaligi") == 0) {
                    research->defense_bonus = level * 10;  // Example scaling for research
                } else if (strcmp(research_type, "saldiri_gelistirmesi") == 0) {
                    research->attack_bonus = level * 10;
                }
                printf("Parsed research: %s, Level: %d\n", research_type, level);  // Debug output for research parsing
            }
        }

        // Move to the next line
        ptr = strchr(ptr, '\n') + 1;
    }
}

// Helper function to read the content of a file into a string
char* readFileContent(const char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *content = (char*)malloc(length + 1);
    if (content) {
        fread(content, 1, length, file);
        content[length] = '\0';  // Null-terminate the string
    }

    fclose(file);
    return content;
}

int main() {
    struct Unit human_units[10000], orc_units[10000];
    int human_unit_count = 0, orc_unit_count = 0;

    struct Hero human_heroes[500], orc_heroes[500];
    int human_hero_count = 0, orc_hero_count = 0;

    struct Creature human_creatures[100], orc_creatures[100];
    int human_creature_count = 0, orc_creature_count = 0;

    struct Research human_research = {0, 0}, orc_research = {0, 0};

    // File names for the 10 JSON files
    const char* files[1] = {
        "files\\1.json",  // Diðer dosyalarý da ekledigimde her zaman ilk olanı okuyo diğerlerini okumuyordu
    };                     // kendimce sacma bir cozum bularak json dosyalarındaki tüm veriyi bir dosyaya attım

    // Loop through each file, read content and parse it
    printf("\nAttempting to open file: %s\n", files[0]);
    char* file_content = readFileContent(files[0]);
    if (file_content != NULL) {
        printf("Parsing file: %s\n", files[0]);

        // Parse the content for both human and orc data
        parseJSON("insan_imparatorlugu", file_content, human_units, &human_unit_count,
                  human_heroes, &human_hero_count, human_creatures, &human_creature_count, &human_research);
        parseJSON("ork_legi", file_content, orc_units, &orc_unit_count,
                  orc_heroes, &orc_hero_count, orc_creatures, &orc_creature_count, &orc_research);

        free(file_content);  // Free the memory allocated for file content

        // Debug output to confirm completion of file parsing
        printf("Completed parsing for file: %s\n\n", files[0]);
    } else {
        printf("Skipping file: %s due to read error.\n", files[0]);
    }



    // Call calculateBattle to simulate the battle
    calculateBattle(human_units, human_unit_count, human_heroes, human_hero_count,
                    human_creatures, human_creature_count, &human_research,
                    orc_units, orc_unit_count, orc_heroes, orc_hero_count,
                    orc_creatures, orc_creature_count, &orc_research);

    return 0;
}void calculateBattle(struct Unit humans[], int human_count, struct Hero human_heroes[], int human_hero_count,
                     struct Creature human_creatures[], int human_creature_count, struct Research *human_research,
                     struct Unit orcs[], int orc_count, struct Hero orc_heroes[], int orc_hero_count,
                     struct Creature orc_creatures[], int orc_creature_count, struct Research *orc_research) {

    FILE *f = fopen("savas_sim.txt", "w");  // Savaþ simülasyonu dosyasýný aç
    if (f == NULL) {
        printf("Error opening file!\n");
        return;
    }

    int round = 1;  // Tur sayacýný baþlat
    float fatigue_factor = 1.0;  // Yorgunluk faktörü, her 5 turda bir azalacak
    float hero_attack_bonus, hero_defense_bonus, creature_bonus, research_attack_bonus, research_defense_bonus;

    while (human_count > 0 && orc_count > 0) {
        fprintf(f, "Round %d\n", round);  // Dosyaya round bilgisi yaz
        printf("Round %d\n", round);

        // 1. Saldýrý ve savunma gücü hesaplamalarý
        float human_total_attack = 0, orc_total_attack = 0;
        float human_total_defense = 0, orc_total_defense = 0;

        // Ýnsan ordusu saldýrý ve savunma hesaplamalarý
        for (int i = 0; i < human_count; i++) {
            // Hero bonusu ve diðer faktörler için baþlangýç
            hero_attack_bonus = 1.0, hero_defense_bonus = 1.0;
            creature_bonus = 1.0;

            // Kahraman bonusu uygula (örneðin Alparslan'ýn savunma bonusu)
            for (int j = 0; j < human_hero_count; j++) {
                if (strcmp(humans[i].name, "piyadeler") == 0 && strcmp(human_heroes[j].name, "Alparslan") == 0) {
                    hero_defense_bonus += human_heroes[j].defense_bonus;
                }
            }

            // Canavar bonusu uygula
            for (int j = 0; j < human_creature_count; j++) {
                creature_bonus += human_creatures[j].bonus_friendly;
            }

            // Araþtýrma bonusu uygula
            research_attack_bonus = 1.0 + human_research->attack_bonus / 100.0;
            research_defense_bonus = 1.0 + human_research->defense_bonus / 100.0;

            // Toplam saldýrý ve savunma gücü
            float attack_with_bonus = humans[i].attack * humans[i].count * hero_attack_bonus * creature_bonus * research_attack_bonus * fatigue_factor;
            float defense_with_bonus = humans[i].defense * humans[i].count * hero_defense_bonus * creature_bonus * research_defense_bonus * fatigue_factor;

            human_total_attack += attack_with_bonus;
            human_total_defense += defense_with_bonus;
        }

        // Ork ordusu saldýrý ve savunma hesaplamalarý
        for (int i = 0; i < orc_count; i++) {
            hero_attack_bonus = 1.0, hero_defense_bonus = 1.0;
            creature_bonus = 1.0;

            // Kahraman bonusu uygula (örneðin Thruk Kemikkýran'ýn troll savunma bonusu)
            for (int j = 0; j < orc_hero_count; j++) {
                if (strcmp(orcs[i].name, "troller") == 0 && strcmp(orc_heroes[j].name, "Thruk Kemikkiran") == 0) {
                    hero_defense_bonus += orc_heroes[j].defense_bonus;
                }
            }

            // Canavar bonusu uygula
            for (int j = 0; j < orc_creature_count; j++) {
                creature_bonus += orc_creatures[j].bonus_friendly;
            }

            // Araþtýrma bonusu uygula
            research_attack_bonus = 1.0 + orc_research->attack_bonus / 100.0;
            research_defense_bonus = 1.0 + orc_research->defense_bonus / 100.0;

            // Toplam saldýrý ve savunma gücü
            float attack_with_bonus = orcs[i].attack * orcs[i].count * hero_attack_bonus * creature_bonus * research_attack_bonus * fatigue_factor;
            float defense_with_bonus = orcs[i].defense * orcs[i].count * hero_defense_bonus * creature_bonus * research_defense_bonus * fatigue_factor;

            orc_total_attack += attack_with_bonus;
            orc_total_defense += defense_with_bonus;
        }

        // 2. Kritik vuruþ hesaplamasý (insanlar ve orklar için)
        for (int i = 0; i < human_count; i++) {
            if (rand() % 100 < humans[i].crit_chance * 100) {  // Kritik vuruþ þansý
                human_total_attack *= 1.5;
            }
        }
        for (int i = 0; i < orc_count; i++) {
            if (rand() % 100 < orcs[i].crit_chance * 100) {
                orc_total_attack *= 1.5;
            }
        }

        // 3. Net hasar hesaplamasý
        float human_damage_to_orcs = human_total_attack * (1 - (orc_total_defense / human_total_attack));
        float orc_damage_to_humans = orc_total_attack * (1 - (human_total_defense / orc_total_attack));

        // 4. Orantýlý hasar daðýlýmý
        for (int i = 0; i < orc_count; i++) {
            float damage_to_ors = (orc_total_defense / human_total_defense) * human_damage_to_orcs;
            orcs[i].health -= damage_to_ors / orcs[i].count;
            if (orcs[i].health <= 0) {
                orcs[i].count = 0;  // Birim yok oldu
            }
        }
        for (int i = 0; i < human_count; i++) {
            float damage_to_humans = (human_total_defense / orc_total_defense) * orc_damage_to_humans;
            humans[i].health -= damage_to_humans / humans[i].count;
            if (humans[i].health <= 0) {
                humans[i].count = 0;  // Birim yok oldu
            }
        }

        // 5. Kalan birimleri kontrol et
        int humans_alive = 0, orcs_alive = 0;
        for (int i = 0; i < human_count; i++) {
            humans_alive += humans[i].count;
        }
        for (int i = 0; i < orc_count; i++) {
            orcs_alive += orcs[i].count;
        }

        // Sonuçlar
        fprintf(f, "Remaining humans: %d, Remaining orcs: %d\n", humans_alive, orcs_alive);
        if (humans_alive == 0) {
            fprintf(f, "Orcs win!\n");
            printf("Orcs win!\n");
            break;
        }
        if (orcs_alive == 0) {
            fprintf(f, "Humans win!\n");
            printf("Humans win!\n");
            break;
        }

        // 6. Yorgunluk etkisi uygula
        if (round % 5 == 0) {
            fatigue_factor *= 0.90;  // %10 azaltma
        }

        round++;
    }

    fclose(f);  // Dosyayý kapat
}

