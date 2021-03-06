#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_LETTERS 26
#define NUMBER_OF_TILES 27
#define MAX_WORD_SIZE 60
#define INVALID_CHARACTER -1
#define LETTER_CASE_DIFFERENCE 32

struct Node {
  int end_word;
  struct Node* next[NUMBER_OF_LETTERS];
};

struct Tile {
  char letter;
  int score;
  int amount;
};

struct Node* get_new_node(){
  struct Node* node = malloc(sizeof(struct Node));
  node->end_word = 0;
  for(int i = 0; i < NUMBER_OF_LETTERS; i++)
    node->next[i] = NULL;
  return node;
}

void insert(struct Node* head, char* word){
  struct Node* node = head;
  while (*word != '\r'){
    if(node->next[*word - 'A'] == NULL)
      node->next[*word - 'A'] = get_new_node();
    node = node->next[*word - 'A'];
    word++;
  }
  node->end_word = 1;
}

void load_words(char* filename, struct Node* head){
  FILE* wordlist = fopen(filename, "r");
  while(1){
    char* word = malloc(sizeof(char) * MAX_WORD_SIZE);
    if(fgets(word, MAX_WORD_SIZE, wordlist) == NULL) break;
    insert(head, word);
  }
}

void add_tile_to_buffer(struct Tile* tile, char* end_of_buffer, char current_letter, int* word_score){
  tile->amount--;
  *word_score += tile->score;
  *end_of_buffer = current_letter;
}

void remove_tile_from_buffer(struct Tile* tile, char* buffer, int *word_score){
  tile->amount++;
  *word_score -= tile->score;
  *buffer = 0;
}

struct Tile* get_tiles(char* letters){
  static struct Tile tiles[NUMBER_OF_TILES];
  int tile_scores[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10, 0};
  int tile_index;
  while(*letters != 0) {
    if(*letters >= 'a' && *letters <= 'z')
      *letters -= LETTER_CASE_DIFFERENCE;
    if(*letters >= 'A' && *letters <= 'Z')
      tile_index = *letters - 'A';
    else if(*letters == '?')
      tile_index = 26;
    else
      tile_index = INVALID_CHARACTER;
    if(tile_index != INVALID_CHARACTER){
      tiles[tile_index].letter = *letters;
      tiles[tile_index].amount++;
      tiles[tile_index].score = tile_scores[tile_index];
    }
    letters++;
  }
  return tiles;
}

void get_anagrams(struct Node* root, struct Tile tiles[], char* buffer, int buffer_pointer, int word_score);

void traverse_trie(struct Node* root, struct Tile tiles[], int tile_pointer, char* buffer, int buffer_pointer, char letter, int word_score){
  struct Node* current_node = root->next[letter - 'A'];
  if(current_node != NULL){
    add_tile_to_buffer(tiles + tile_pointer, buffer + buffer_pointer, letter, &word_score);
    if(current_node->end_word) printf("%s %d\n", buffer, word_score);
    get_anagrams(current_node, tiles, buffer, buffer_pointer + 1, word_score);
    remove_tile_from_buffer(tiles + tile_pointer, buffer + buffer_pointer, &word_score);
  }
}

void get_anagrams(struct Node* root, struct Tile tiles[], char* buffer, int buffer_pointer, int word_score){
  struct Node* current_node;
  struct Tile* tile;
  char current_letter;
  char start_letter;
  char end_letter;
  for(int tile_pointer = 0; tile_pointer < NUMBER_OF_TILES; tile_pointer++){
    current_letter = tiles[tile_pointer].letter;
    if(current_letter != '?'){
      start_letter = current_letter;
      end_letter = current_letter;
    }
    else{
      start_letter = 'A';
      end_letter = 'Z';
    }
    for(current_letter = start_letter; current_letter <= end_letter; current_letter++){
      if(tiles[tile_pointer].amount > 0)
        traverse_trie(root, tiles, tile_pointer, buffer, buffer_pointer, current_letter, word_score);
    }
  }
}

int main(int argc, char *argv[]){
  if(argc != 2)
    printf("One argument expected.\n");
  else{
    char* letters = argv[1];
    struct Tile* tiles = get_tiles(letters);
    char* buffer = malloc(sizeof(char) * MAX_WORD_SIZE);
    struct Node* root = malloc(sizeof(struct Node));
    load_words("sowpods.txt", root);
    get_anagrams(root, tiles, buffer, 0, 0);
  }
  return 0;
}
