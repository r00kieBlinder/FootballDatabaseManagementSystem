#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define INT_MAX 100
#define MAX_TEAMS 10
#define MAX_PLAYERS_PER_TEAM 20
#define MAX_NAME_LENGTH 50
#define FILENAME_TEAMS "football_database.txt"
#define FILENAME_STATS "stats.txt"


struct Player {
    char playerName[50];
    int playerNumber;
    char playerPosition[10];
    int goalsScored;
    int goalsConceded;
    int minutes_played;
};

struct Team {
    char team_name[MAX_NAME_LENGTH];
    int num_players;
    struct Player players[MAX_PLAYERS_PER_TEAM];
    int goalsScored;
    int goalsConceded;
};

void saveTeamsToFile(const struct Team* teams, int num_teams) {
    FILE* file = fopen(FILENAME_TEAMS, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_teams; ++i) {
        fprintf(file, "%s %d\n", teams[i].team_name, teams[i].num_players);
        for (int j = 0; j < teams[i].num_players; ++j) {
            fprintf(file, "%s %d %s %d %d %d\n",
                    teams[i].players[j].playerName,
                    teams[i].players[j].playerNumber,
                    teams[i].players[j].playerPosition,
                    teams[i].players[j].goalsScored,
                    teams[i].players[j].goalsConceded,
                    teams[i].players[j].minutes_played);
        }
    }

    fclose(file);
}

void loadTeamsFromFile(struct Team* teams, int* num_teams) {
    FILE* file = fopen(FILENAME_TEAMS, "r");
    if (file == NULL) {
        printf("Error opening file for reading. Starting with an empty database.\n");
        return;
    }

    while (*num_teams < MAX_TEAMS && fscanf(file, "%s %d",
            teams[*num_teams].team_name, &teams[*num_teams].num_players) == 2) {
        for (int i = 0; i < teams[*num_teams].num_players; ++i) {
            struct Player* player = &teams[*num_teams].players[i];
            fscanf(file, "%s %d %s %d %d %d",
                    player->playerName,
                    &player->playerNumber,
                    player->playerPosition,
                    &player->goalsScored,
                    &player->goalsConceded,
                    &player->minutes_played);
        }
        (*num_teams)++;
    }

    fclose(file);
}


void addPlayerToTeam(struct Team* team, const struct Player* player) {
    if (team->num_players < MAX_PLAYERS_PER_TEAM) {
        team->players[team->num_players] = *player;
        team->num_players++;
        printf("Player added to the team successfully.\n");
    } else {
        printf("Team is full. Cannot add more players.\n");
    }
}

void displayPlayer(const struct Player* player) {
    printf("Player Name: %s\n", player->playerName);
    printf("Player Number: %d\n", player->playerNumber);
    printf("Player Position: %s\n", player->playerPosition);
    printf("Goals Scored: %d\n", player->goalsScored);
    printf("Goals Conceded: %d\n", player->goalsConceded);
    printf("Minutes Played: %d\n", player->minutes_played);
}

void displayTeam(const struct Team* team) {
    printf("Team Name: %s\n", team->team_name);
    printf("Number of Players: %d\n", team->num_players);
    for (int i = 0; i < team->num_players; ++i) {
        displayPlayer(&team->players[i]);
        printf("\n");
    }
}

void updateTeam(struct Team* team, const char* position) {
    for (int i = 0; i < team->num_players; ++i) {
        if (strcmp(team->players[i].playerPosition, position) == 0) {
            if (strcmp(position, "MID") == 0 || strcmp(position, "FWD") == 0) {
                team->goalsScored += team->players[i].goalsScored;
            } else if (strcmp(position, "DEF") == 0) {
                team->goalsConceded += team->players[i].goalsConceded;
            }
        }
    }
}

void displayStatistics(const struct Team* teams, int num_teams) {
    int maxGoalsScored = 0;
    int minGoalsConceded = INT_MAX;
    char teamMostGoalsScored[MAX_NAME_LENGTH];
    char teamLeastGoalsConceded[MAX_NAME_LENGTH];

    for (int i = 0; i < num_teams; ++i) {
        if (teams[i].goalsScored > maxGoalsScored) {
            maxGoalsScored = teams[i].goalsScored;
            strcpy(teamMostGoalsScored, teams[i].team_name);
        }

        if (teams[i].goalsConceded < minGoalsConceded) {
            minGoalsConceded = teams[i].goalsConceded;
            strcpy(teamLeastGoalsConceded, teams[i].team_name);
        }
    }

    FILE* statsFile = fopen(FILENAME_STATS, "w");
    if (statsFile == NULL) {
        printf("Error opening stats file for writing.\n");
        return;
    }

    fprintf(statsFile, "Most goals scored: %s %d\n", teamMostGoalsScored, maxGoalsScored);
    fprintf(statsFile, "Least goals conceded: %s %d\n", teamLeastGoalsConceded, minGoalsConceded);

    fclose(statsFile);

    printf("Statistics saved to %s\n", FILENAME_STATS);
}





int main() {
    struct Team footballTeams[MAX_TEAMS];
    int numTeams = 0;
    loadTeamsFromFile(footballTeams, &numTeams);

    int choice;
    while(1){
        printf("----------------------------------------------------------------------\n");
        printf("----------Welcome to Football Management System (FMS)-----------------\n");
        printf("----------------------------------------------------------------------\n\n");
        printf("Menu:\n1.Add a player\n2.Display Team information\n3.Update team information\n4.Show Statistics\n5.Save Team\n6.Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);
        switch(choice){
        case 1:
            {
        struct Player newPlayer;
        printf("Enter Player Name: ");
        scanf("%s", newPlayer.playerName);
        printf("Enter Player Number: ");
        scanf("%d", &newPlayer.playerNumber);
        printf("Enter Player Position: ");
        scanf("%s", newPlayer.playerPosition);
        printf("Enter Goals Scored: ");
        scanf("%d", &newPlayer.goalsScored);
        printf("Enter Goals Conceded: ");
        scanf("%d", &newPlayer.goalsConceded);
        printf("Enter Minutes Played: ");
        scanf("%d", &newPlayer.minutes_played);

        printf("Enter Team Index: ");
        int teamIndex;
        scanf("%d", &teamIndex);

        if (teamIndex >= 0 && teamIndex < numTeams) {
            addPlayerToTeam(&footballTeams[teamIndex], &newPlayer);
        }
        break;
    }

        case 2:
             {
        printf("Enter Team Index: ");
        int teamIndex;
        scanf("%d", &teamIndex);

        if (teamIndex >= 0 && teamIndex < numTeams) {
            displayTeam(&footballTeams[teamIndex]);
        } else {
            printf("Invalid Team Index.\n");
        }
        break;
    }
        case 3:
            {
        printf("Enter Team Index: ");
        int teamIndex;
        scanf("%d", &teamIndex);

        if (teamIndex >= 0 && teamIndex < numTeams) {
            char positionToUpdate[10];
            printf("Enter Player Position to Update: ");
            scanf("%s", positionToUpdate);
            updateTeam(&footballTeams[teamIndex], positionToUpdate);
        } else {
            printf("Invalid Team Index.\n");
        }
        break;
    }
           
        case 4:
            {
        displayStatistics(footballTeams, numTeams);
        break;
    }
        case 5:
            saveTeamsToFile(footballTeams, numTeams);
            printf("Teams saved to %s\n", FILENAME_TEAMS);
            break;
        case 6:
            printf("Thanks for using our System!!!");
            exit(0);
            break;
        }
    }
    return 0;
}