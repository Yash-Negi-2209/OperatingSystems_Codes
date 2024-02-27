#include <stdio.h>
#include <stdlib.h>
// Function to cross the river
void crossRiver(char* people, int size);
// Function to check if a character is present in an array
int contains(char* people, int size, char target);
// Function to cross the river
void crossRiver(char* people, int size) {
if (size == 0) {
printf("Everyone has safely crossed the river.\n");
return;
}
printf("Current state: ");
for (int i = 0; i < size; ++i) {
printf("%c ", people[i]);
}
printf("\n");
for (int i = 0; i < size; ++i) {
for (int j = i + 1; j < size; ++j) {
// Create a new state by removing two people from the current state
char* newState = (char*) malloc((size - 2) * sizeof(char));
int index = 0;
for (int k = 0; k < size; ++k) {
if (k != i && k != j) {
newState[index++] = people[k];
}
}
// Check if the new state is valid
if (!(contains(newState, size - 2, 'c') && contains(newState, size - 2, 'm')) &&
!(contains(newState, size - 2, 'd') && contains(newState, size - 2, 'c'))) {
printf("Move: %c and %c\n", people[i], people[j]);
// Recursively cross the river with the new state
crossRiver(newState, size - 2);
}
free(newState); // Free the memory allocated for the new state
}
}
}
// Function to check if a character is present in an array
int contains(char* people, int size, char target) {
for (int i = 0; i < size; ++i) {
if (people[i] == target) {
return 1;
}
}
return 0;
}
int main() {
char people[] = {'a', 'b', 'c', 'd', 'm'};
int size = sizeof(people) / sizeof(people[0]);
printf("Starting the river crossing problem.\n");
crossRiver(people, size);
return 0;
}