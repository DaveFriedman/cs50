#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
// i: column of candidates
// j: rows of number of voters who prefer candidate i over candidate j
// meaning: preferences[0][1] is preferences[Alice][Bob], which is the number
//     of voters who prefer Alice to Bob
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        // Array of preferences is in candidate order as listed in argc
        // meaning: rank[0] is rank[Alice], rank[1] is rank[Bob], etc.
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");

        // print ranks[]
        for (int k = 0; k < candidate_count; k++)
        {
            printf("ranks[%i] %i \n", k, ranks[k]);
        }
    }

        // print preferences[][]
        printf("preferences[][] final: \n");
        for (int m = 0; m < candidate_count; m++)
        {
            for (int n = 0; n < candidate_count; n++)
            {
                printf("%i ", preferences[m][n]);
            }
            printf("\n");
        }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // for each candidate in candidates[]
    for (int c = 0; c < candidate_count; c++)
    {
        // if string name matches a candidate's name
        if (strcmp(name, candidates[c]) == 0)
        {
            // then represent the voter's ballot in ranks[].
            // ranks[] is every candidate's index, ordered from the voter's
            // first choice to last.
            // If candidates[] = {Alice, Bob, Charlie}, and voter 1 ranks them
            // Bob Alice Charlie, then ranks[] = {1, 0, 2}
            // ranks[0] = 1, because Bob, in candidates[1], is the voter's 1st
            // choice, which is ranks[0]
            ranks[rank] = c;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // preferences[row][col] represents the number
    // of voters who prefer candidate[row] over candidate[col]
    // So, for each position on the ballot
    printf("preferences[][]\n");
    for (int i = 0; i < candidate_count; i++)
    {
        // and for each other position on the ballot
        for (int j = i + 1; j < candidate_count; j++)
        {
            // if the index of candidate in the ith position of ranks is
            // less than the index of
            if (ranks[i] < ranks[j])
            {
                preferences[ ranks[i] ][ranks[j]]++;
            }
            printf("%i ", preferences [ranks[i]] [ranks[j]] );
        }
        printf("\n");
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    return;
}

