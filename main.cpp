#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_RECIPES 100
#define MAX_NAME 50
#define MAX_DESC 200
#define MAX_INGREDIENTS 300
#define FILE_NAME "recipes.txt"

#define COLOR_RESET "\033[0m"
#define COLOR_PINK "\033[38;5;213m"
#define COLOR_PURPLE "\033[38;5;135m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_RED "\033[0;31m"
#define COLOR_GREEN "\033[0;32m"

void clear_screen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

typedef struct {
    char name[MAX_NAME];
    char description[MAX_DESC];
    char ingredients[MAX_INGREDIENTS];
} Recipe;

Recipe recipes[MAX_RECIPES];
int recipe_count = 0;

void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void read_line(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = 0;
}

void save_recipes() {
    FILE *file = fopen(FILE_NAME, "w");
    if (!file) {
        printf(COLOR_RED "Error saving recipes!\n" COLOR_RESET);
        return;
    }
    for (int i = 0; i < recipe_count; i++) {
        fprintf(file, "%s\n%s\n%s\n", recipes[i].name, recipes[i].description, recipes[i].ingredients);
    }
    fclose(file);
}

void load_recipes() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) return;
    while (fgets(recipes[recipe_count].name, MAX_NAME, file) &&
           fgets(recipes[recipe_count].description, MAX_DESC, file) &&
           fgets(recipes[recipe_count].ingredients, MAX_INGREDIENTS, file)) {
        recipes[recipe_count].name[strcspn(recipes[recipe_count].name, "\n")] = 0;
        recipes[recipe_count].description[strcspn(recipes[recipe_count].description, "\n")] = 0;
        recipes[recipe_count].ingredients[strcspn(recipes[recipe_count].ingredients, "\n")] = 0;
        recipe_count++;
    }
    fclose(file);
}

void prepopulate_recipes() {
    FILE *file = fopen(FILE_NAME, "r");
    if (!file) {
        file = fopen(FILE_NAME, "w");
        if (!file) {
            printf(COLOR_RED "Error creating recipes file!\n" COLOR_RESET);
            return;
        }
        fprintf(file, "Pancakes\nFluffy homemade pancakes\nFlour, eggs, milk, sugar, baking powder\n");
        fprintf(file, "Spaghetti Bolognese\nClassic Italian pasta with meat sauce\nSpaghetti, minced meat, tomatoes, onion, garlic\n");
        fclose(file);
    }
}

void header() {
    printf(COLOR_PURPLE "\n=============== " COLOR_PINK "My Recipe Book" COLOR_PURPLE " ===============\n" COLOR_RESET);
    printf("1 - Add Recipe\n");
    printf("2 - Display Recipes\n");
    printf("3 - Search Recipe\n");
    printf("4 - Delete Recipe\n");
    printf("5 - Edit Recipe\n");
    printf("0 - Exit\n");
    printf("---------------------------------------------\n");
}

void add_recipe() {
    if (recipe_count >= MAX_RECIPES) {
        printf(COLOR_RED "Recipe book is full!\n" COLOR_RESET);
        return;
    }
    flush_input();
    read_line("✅ Enter recipe name: ", recipes[recipe_count].name, MAX_NAME);
    read_line("✅ Enter recipe description: ", recipes[recipe_count].description, MAX_DESC);
    read_line("✅ Enter ingredients (comma separated): ", recipes[recipe_count].ingredients, MAX_INGREDIENTS);

    recipe_count++;
    save_recipes();
    printf(COLOR_GREEN "Recipe added successfully!\n" COLOR_RESET);
}

void display_recipes() {
    if (recipe_count == 0) {
        printf(COLOR_YELLOW "No recipes available!\n" COLOR_RESET);
        return;
    }
    printf("\n--- " COLOR_PURPLE "Recipe Book" COLOR_RESET " ---\n");
    for (int i = 0; i < recipe_count; i++) {
        printf("%d. " COLOR_PINK "%s" COLOR_RESET "\n   " COLOR_PURPLE "Description: %s" COLOR_RESET "\n   " COLOR_PINK "Ingredients: %s" COLOR_RESET "\n",
               i + 1, recipes[i].name, recipes[i].description, recipes[i].ingredients);
    }
    printf("-------------------\n");
}

void search_recipe() {
    char keyword[MAX_NAME];
    flush_input();
    read_line("✅ Enter recipe name to search: ", keyword, MAX_NAME);
    printf("Searching for: '%s'\n", keyword);

    for (int i = 0; i < recipe_count; i++) {
        if (strstr(recipes[i].name, keyword)) {
            printf("Found: " COLOR_PINK "%s" COLOR_RESET "\n   " COLOR_PURPLE "Description: %s" COLOR_RESET "\n   " COLOR_PINK "Ingredients: %s" COLOR_RESET "\n",
                   recipes[i].name, recipes[i].description, recipes[i].ingredients);
            return;
        }
    }
    printf(COLOR_RED "Recipe not found!\n" COLOR_RESET);
}

void delete_recipe() {
    int index;
    printf("✅ Enter recipe number to delete (1 - %d): ", recipe_count);
    if (scanf("%d", &index) != 1 || index < 1 || index > recipe_count) {
        printf(COLOR_RED "🚫 Invalid recipe number!\n" COLOR_RESET);
        flush_input();
        return;
    }
    index--;
    for (int i = index; i < recipe_count - 1; i++) {
        recipes[i] = recipes[i + 1];
    }
    recipe_count--;
    save_recipes();
    printf(COLOR_GREEN "Recipe deleted successfully!\n" COLOR_RESET);
}

void edit_recipe() {
    int index;
    printf("✅ Enter recipe number to edit (1 - %d): ", recipe_count);
    if (scanf("%d", &index) != 1 || index < 1 || index > recipe_count) {
        printf(COLOR_RED "🚫 Invalid recipe number!\n" COLOR_RESET);
        flush_input();
        return;
    }
    index--;
    flush_input();
    read_line("✅ Enter new recipe name: ", recipes[index].name, MAX_NAME);
    read_line("✅ Enter new description: ", recipes[index].description, MAX_DESC);
    read_line("✅ Enter new ingredients: ", recipes[index].ingredients, MAX_INGREDIENTS);

    save_recipes();
    printf(COLOR_GREEN "Recipe updated successfully!\n" COLOR_RESET);
}

void menu(int op) {
    switch(op) {
        case 1:
            add_recipe(); break;
        case 2:
            display_recipes(); break;
        case 3:
            search_recipe(); break;
        case 4:
            delete_recipe(); break;
        case 5:
            edit_recipe(); break;
        default:
            printf("EXIT!\n");
    }
}

int main() {
    prepopulate_recipes();
    load_recipes();
    int option;
    do {
        header();
        printf("Enter option: ");
        if (scanf("%d", &option) != 1) {
            printf(COLOR_RED "🚫 Invalid input. Please enter a number.\n" COLOR_RESET);
            flush_input();
            continue;
        }
        clear_screen();
        menu(option);
        if (option != 0) {
            printf("\nPress Enter to return to the menu...");
            flush_input();
            getchar();
            clear_screen();
        }
    } while(option != 0);
    return 0;
}
