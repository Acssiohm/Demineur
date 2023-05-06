// #include "game.h"

// #include <iostream>
// #include <stdbool.h>
// #include <time.h>

// // Construit une grille de jeu démineur carré taille * taille, où il y aura
// // nombrebombes bombes.
// Game::Game(int taille, int nombrebombes) {
//   n = taille;
//   nbrbombes = nombrebombes;
//   tabc = generertableau(-2);
//   nbrcasescaches = taille * taille;
// }

// // Révèle la case (tx, ty), en révélant récursivement si la case est vide,
// // toutes les cases autour. Renvoie l'état de la game après le coup :
// // - 0 : état quelconque, la partie continue
// // - -1 : la partie est terminé, une bombe se cachait sous la case
// // - 1 : Le coup était inutile (déjà révélé, drapeau)
// // - 2 : Le coup a mis fin a la partie, le joueur a révélé toutes les cases
// // n'étant pas des bombes.
// int Game::Coup(int tx, int ty) {
//   if (!generated) {
//     tabr = generergrille(tx, ty);
//   }
//   if (tabc[tx][ty] != -2 || tabc[tx][ty] == -1) {
//     return 1; // La case est déjà révélée ou marquée par un drapeau
//   }
//   if (tabr[tx][ty] == -1) {
//     return -1; // La case est minée
//   }
//   reveler(tx, ty);
//   if (nbrcasescaches == nbrbombes) {
//     return 2;
//   }
//   return 0;
// }

// // Renvoie une copie de la partie identique dans le fond, mais séparé en terme
// // de mémoire
// Game Game::Copy() {
//   int **tabreel = (int **)malloc(sizeof(int *) * n);
//   int **tabconnu = (int **)malloc(sizeof(int *) * n);

//   for (int i = 0; i < n; i++) {
//     tabreel[i] = (int *)malloc(sizeof(int) * n);
//     tabconnu[i] = (int *)malloc(sizeof(int) * n);
//     for (int j = 0; j < n; j++) {
//       tabreel[i][j] = tabr[i][j];
//       tabconnu[i][j] = tabc[i][j];
//     }
//   }
//   return Game(n, nbrbombes, tabreel, tabconnu, nbrcasescaches);
// }

// Game::~Game() {
//   freetableau(tabr);
//   freetableau(tabc);
// }

// // Renvoie l'état visible de la partie
// int **Game::GetGame() { return tabc; }

// // Affiche dans la console l'état visible de la partie
// void Game::PrintGame() { printtableau(tabc); }

// // Joue dans la console une partie de démineur en taille 11*11 avec 10 bombes
// void Game::GameConsole() {
//   Game game = Game(11, 10);
//   game.PrintGame();
//   int tx;
//   int ty;
//   std::cout << "Coordonée x : ";
//   std::cin >> tx;
//   std::cout << "Coordonée y : ";
//   std::cin >> ty;

//   game.Coup(tx, ty);

//   while (1) {
//     printf("---\n");
//     game.PrintGame();
//     printf("Coordonée x : ");
//     scanf("%u", &tx);
//     printf("Coordonée y : ");
//     scanf("%u", &ty);
//     if (game.Coup(tx, ty) == -1) {
//       break;
//     }
//   }
//   printf("Vous avez perdu");
// }

// // Attention, ce constructeur présuppose que la partie a déjà commencé
// // (minimum le coup de départ)
// Game::Game(int taille, int nombrebombes, int **tabreel, int **tabconnu,
//            int nbrcasesinconnues) {
//   n = taille;
//   nbrbombes = nombrebombes;
//   tabc = tabconnu;
//   tabr = tabreel;
//   nbrcasescaches = nbrcasesinconnues;
//   generated = true;
// }

// int **Game::generertableau(int x) {
//   int **tab = (int **)malloc(sizeof(int *) * n);
//   for (int i = 0; i < n; ++i) {
//     tab[i] = (int *)malloc(sizeof(int) * n);
//     for (int j = 0; j < n; ++j) {
//       tab[i][j] = x;
//     }
//   }
//   return tab;
// }

// int **Game::generergrille(int x0, int y0) {
//   srand(time(NULL));
//   int **tab = generertableau(0);
//   int i = 0;
//   while (i < nbrbombes) {
//     int x = rand() % n;
//     int y = rand() % n;
//     if (x == x0 && y == y0) {
//       continue;
//     }

//     if (tab[x][y] != -1) {
//       tab[x][y] = -1;
//       if (x == 0) {
//         if (y == 0) {
//           if (tab[x + 1][y] != -1) {
//             tab[x + 1][y]++;
//           }

//           if (tab[x][y + 1] != -1) {
//             tab[x][y + 1]++;
//           }

//           if (tab[x + 1][y + 1] != -1) {
//             tab[x + 1][y + 1]++;
//           }
//         } 
//         else if (y == n - 1) {
//           if (tab[x][y - 1] != -1) {
//             tab[x][y - 1]++;
//           }

//           if (tab[x + 1][y - 1] != -1) {
//             tab[x + 1][y - 1]++;
//           }

//           if (tab[x + 1][y] != -1) {
//             tab[x + 1][y]++;
//           }
//         } 
//         else {

//           if (tab[x][y - 1] != -1) {
//             tab[x][y - 1]++;
//           }

//           if (tab[x + 1][y - 1] != -1) {
//             tab[x + 1][y - 1]++;
//           }

//           if (tab[x + 1][y] != -1) {
//             tab[x + 1][y]++;
//           }

//           if (tab[x][y + 1] != -1) {
//             tab[x][y + 1]++;
//           }

//           if (tab[x + 1][y + 1] != -1) {
//             tab[x + 1][y + 1]++;
//           }
//         }
//       } 
//       else if (x == n - 1) {
//         if (y == 0) {
//           if (tab[x - 1][y] != -1) {
//             tab[x - 1][y]++;
//           }

//           if (tab[x - 1][y + 1] != -1) {
//             tab[x - 1][y + 1]++;
//           }

//           if (tab[x][y + 1] != -1) {
//             tab[x][y + 1]++;
//           }
//         } 
//         else if (y == n - 1) {
//           if (tab[x - 1][y - 1] != -1) {
//             tab[x - 1][y - 1]++;
//           }

//           if (tab[x][y - 1] != -1) {
//             tab[x][y - 1]++;
//           }

//           if (tab[x - 1][y] != -1) {
//             tab[x - 1][y]++;
//           }
//         } 
//         else {
//           if (tab[x - 1][y - 1] != -1) {
//             tab[x - 1][y - 1]++;
//           }

//           if (tab[x][y - 1] != -1) {
//             tab[x][y - 1]++;
//           }

//           if (tab[x - 1][y] != -1) {
//             tab[x - 1][y]++;
//           }

//           if (tab[x - 1][y + 1] != -1) {
//             tab[x - 1][y + 1]++;
//           }

//           if (tab[x][y + 1] != -1) {
//             tab[x][y + 1]++;
//           }
//         }
//       } 
//       else {
//         if (y == 0) {
//           if (tab[x - 1][y] != -1) {
//             tab[x - 1][y]++;
//           }

//           if (tab[x + 1][y] != -1) {
//             tab[x + 1][y]++;
//           }

//           if (tab[x - 1][y + 1] != -1) {
//             tab[x - 1][y + 1]++;
//           }

//           if (tab[x][y + 1] != -1) {
//             tab[x][y + 1]++;
//           }

//           if (tab[x + 1][y + 1] != -1) {
//             tab[x + 1][y + 1]++;
//           }
//         }
//         else if (y == n - 1) {
//           if (tab[x - 1][y - 1] != -1) {
//             tab[x - 1][y - 1]++;
//           }

//           if (tab[x][y - 1] != -1) {
//             tab[x][y - 1]++;
//           }

//           if (tab[x + 1][y - 1] != -1) {
//             tab[x + 1][y - 1]++;
//           }

//           if (tab[x - 1][y] != -1) {
//             tab[x - 1][y]++;
//           }

//           if (tab[x + 1][y] != -1) {
//             tab[x + 1][y]++;
//           }
//         } 
//         else {
//           if (tab[x - 1][y - 1] != -1) {
//             tab[x - 1][y - 1]++;
//           }

//           if (tab[x][y - 1] != -1) {
//             tab[x][y - 1]++;
//           }

//           if (tab[x + 1][y - 1] != -1) {
//             tab[x + 1][y - 1]++;
//           }

//           if (tab[x - 1][y] != -1) {
//             tab[x - 1][y]++;
//           }

//           if (tab[x + 1][y] != -1) {
//             tab[x + 1][y]++;
//           }

//           if (tab[x - 1][y + 1] != -1) {
//             tab[x - 1][y + 1]++;
//           }

//           if (tab[x][y + 1] != -1) {
//             tab[x][y + 1]++;
//           }

//           if (tab[x + 1][y + 1] != -1) {
//             tab[x + 1][y + 1]++;
//           }
//         }
//       }
//       i++;
//     }
//   }
//   return tab;
// }

// void Game::printtableau(int **tab) {
//   for (int i = 0; i < n; ++i) {
//     if (i != 0) {
//       std::cout << "\n";
//     }
//     for (int j = 0; j < n; ++j) {
//       if (tab[i][j] >= 0) {
//         std::cout << " ";
//       }
//       std::cout << tab[i][j];
//     }
//     std::cout << std::endl;
//   }
// }

// void Game::freetableau(int **tab) {
//   for (int i = 0; i < n; ++i) {
//     free(tab[i]);
//   }
//   free(tab);
// }

// void Game::reveler(int tx, int ty) {
//   int tc = tabc[tx][ty];
//   tabc[tx][ty] = tabr[tx][ty];
//   nbrcasescaches--;
//   if (tabr[tx][ty] == 0 && tc == -2) {
//     if (tx == 0) {
//       if (ty == 0) {
//         reveler(tx, ty + 1);
//         reveler(tx + 1, ty);
//         reveler(tx + 1, ty + 1);

//       } else if (ty == n - 1) {
//         reveler(tx, ty - 1);
//         reveler(tx + 1, ty - 1);
//         reveler(tx + 1, ty);

//       } else {
//         reveler(tx, ty - 1);
//         reveler(tx, ty + 1);
//         reveler(tx + 1, ty - 1);
//         reveler(tx + 1, ty);
//         reveler(tx + 1, ty + 1);
//       }
//     } else if (tx == n - 1) {
//       if (ty == 0) {
//         reveler(tx - 1, ty);
//         reveler(tx - 1, ty + 1);
//         reveler(tx, ty + 1);

//       } else if (ty == n - 1) {
//         reveler(tx - 1, ty - 1);
//         reveler(tx - 1, ty);
//         reveler(tx, ty - 1);

//       } else {
//         reveler(tx - 1, ty - 1);
//         reveler(tx - 1, ty);
//         reveler(tx - 1, ty + 1);
//         reveler(tx, ty - 1);
//         reveler(tx, ty + 1);
//       }
//     } else {
//       if (ty == 0) {
//         reveler(tx - 1, ty);
//         reveler(tx - 1, ty + 1);
//         reveler(tx, ty + 1);
//         reveler(tx + 1, ty);
//         reveler(tx + 1, ty + 1);

//       } else if (ty == n - 1) {
//         reveler(tx - 1, ty - 1);
//         reveler(tx - 1, ty);
//         reveler(tx, ty - 1);
//         reveler(tx + 1, ty - 1);
//         reveler(tx + 1, ty);

//       } else {
//         reveler(tx - 1, ty - 1);
//         reveler(tx - 1, ty);
//         reveler(tx - 1, ty + 1);
//         reveler(tx, ty - 1);
//         reveler(tx, ty + 1);
//         reveler(tx + 1, ty - 1);
//         reveler(tx + 1, ty);
//         reveler(tx + 1, ty + 1);
//       }
//     }
//   }
// }

// // Construit une grille hypothétique de démineur carré taille * taille, où il
// // y aura nombrebombes bombes.
// // Renvoie une position de départ sans aucune information
// GameHypo::GameHypo(int taille, int nombrebombes) {
//   n = taille;
//   nbrbombes = nombrebombes;
//   tabc = generertableau(-2);
//   nbrcasescaches = taille * taille;
// }

// // Un coup dans une game de ce type consiste en fait à déclarer une bombe
// // Renvoie :
// //  - 0 si la grille ne devient pas incohérente
// //  - 1 s'il y a un problème
// int GameHypo::Coup(int tx, int ty) {
//   tabc[tx][ty] = -1;
//   int a = IterationCoup();
//   while (a == 0) {
//     a = IterationCoup();
//   }
//   if (a == -1) {
//     return 1;
//   }
//   return 0;
// }

// // Renvoie une copie de la partie identique dans le fond, mais séparé en terme
// // de mémoire
// GameHypo GameHypo::Copy() {
//   int **tabconnu = (int **)malloc(sizeof(int *) * n);

//   for (int i = 0; i < n; i++) {
//     tabconnu[i] = (int *)malloc(sizeof(int) * n);
//     for (int j = 0; j < n; j++) {
//       tabconnu[i][j] = tabc[i][j];
//     }
//   }
//   return GameHypo(n, nbrbombes, tabconnu, nbrcasescaches);
// }

// GameHypo::~GameHypo() { freetableau(tabc); }

// // Renvoie l'état visible de la partie
// int **GameHypo::GetGame() { return tabc; }

// // Affiche dans la console l'état visible de la partie
// void GameHypo::PrintGame() { printtableau(tabc); }

// // Essaye d'extraire un maximum d'informations de la grille connue
// //(En fait je sais pas trop si c'est nécessaire mais bon)
// // Renvoie :
// //  - 1 si la grille est impossible
// //  - 0 si une information a été déduite
// //  - -1 si c'est un point fixe
// int GameHypo::IterationCoup() {
//   bool modif = false;
//   for (int x = 0; x < n; x++) {
//     for (int y = 0; y < n; y++) {
//       int bombes = 0;
//       int incertitude = 0;
//       for (int i = min(x - 1, 0); i < max(x + 1, n - 1); i++) {
//         for (int j = min(y - 1, 0); i < max(y + 1, n - 1); j++) {
//           if (i == j) {
//             continue;
//           } else if (tabc[i][j] == -1) {
//             bombes++;
//           } else if (tabc[i][j] == -2) {
//             incertitude++;
//           }
//         }
//       }
//       if (tabc[x][y] >= 0) {
//         if (bombes + incertitude > tabc[x][y]) {
//           return 1;
//         }
//       } else if (incertitude == 0) {
//         tabc[x][y] = bombes;
//         modif = true;
//       }
//     }
//   }
//   if (modif) {
//     return 0;
//   }
//   return -1;
// }

// // Attention, ce constructeur présuppose que la partie a déjà commencé
// // (minimum le coup de départ)
// GameHypo::GameHypo(int taille, int nombrebombes, int **tabconnu,
//                    int nbrcasesinconnues) {
//   n = taille;
//   nbrbombes = nombrebombes;
//   tabc = tabconnu;
//   nbrcasescaches = nbrcasesinconnues;
// }
// int **tabc;
// int nbrcasescaches;

// int **GameHypo::generertableau(int x) {
//   int **tab = (int **)malloc(sizeof(int *) * n);
//   for (int i = 0; i < n; ++i) {
//     tab[i] = (int *)malloc(sizeof(int) * n);
//     for (int j = 0; j < n; ++j) {
//       tab[i][j] = x;
//     }
//   }
//   return tab;
// }

// // Maximum de a et b
// int GameHypo::max(int a, int b) {
//   if (a > b) {
//     return a;
//   }
//   return b;
// }
// // Minimum de a et b
// int GameHypo::min(int a, int b) {
//   if (a < b) {
//     return a;
//   }
//   return b;
// }

// void GameHypo::printtableau(int **tab) {
//   for (int i = 0; i < n; ++i) {
//     if (i != 0) {
//       printf("\n");
//     }
//     for (int j = 0; j < n; ++j) {
//       if (tab[i][j] >= 0) {
//         printf(" %d ", tab[i][j]);
//       } else {
//         printf("%d ", tab[i][j]);
//       }
//     }
//     printf("\n");
//   }
// }

// void GameHypo::freetableau(int **tab) {
//   for (int i = 0; i < n; ++i) {
//     free(tab[i]);
//   }
//   free(tab);
// }

// /*int main() {
// Game::GameConsole();
// return 0;
// }*/
