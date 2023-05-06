#pragma once
#include <unordered_set>
#include <vector>
#include <iostream>
#include <random>
#include "SDL_API.h"

class Plateau {
  int m_nb_bombs;
  int m_size;
  std::vector<int> m_grille;
  std::random_device m_dev;
  std::mt19937 m_rand_gen;

public:
  Plateau(int size):m_size(size), m_nb_bombs(0), m_grille(m_size*m_size, 0),m_dev(),m_rand_gen(m_dev())  {
  }
  Plateau(int size, int nb_bombs):Plateau(size){ addBombs(nb_bombs); }

  int get_size() const {
    return m_size;
  }
  int get_nb_bombs() const {
    return m_nb_bombs;
  }
  int get(int i) const {
    return m_grille[i];
  }
  int get(int x, int y) const {
    return get(pos_to_index(x, y));
  }
  void reset(){
    m_nb_bombs = 0;
    for (int i = 0; i < m_grille.size(); ++i){
      m_grille[i] = 0;
    }
  }

  int pos_to_index(int x, int y) const {
    return x + m_size*y;
  }

  bool is_in_grille(int x, int y) const {
    return (x >= 0)&&(x < m_size)&&(y >= 0)&&(y < m_size);
  }

  vector<int> voisins(int x, int y) const {
    vector<int> result;
    for(int x_offset : {-1, 0, 1}){
      for(int y_offset : {-1, 0, 1}){
        if(x_offset == 0 && y_offset == 0){
          continue;
        }
        if(is_in_grille(x + x_offset, y + y_offset)){
          result.push_back( pos_to_index(x + x_offset, y + y_offset) );
        }
      }
    }
    return result;
  }
  
  vector<int> voisins(int i) const {
    return voisins( i%m_size, i/m_size );
  }
  
  bool isBomb(int i) const {
    return m_grille[i] == -1;
  }
  int isBomb(int x, int y) const {
    return isBomb(pos_to_index(x, y));
  }
  void removeBomb(int i){
    if(!isBomb(i)){
      return;
    }
    m_grille[i] = 0;
    m_nb_bombs--;
    for(int v : voisins(i)){
      if(!isBomb(v)){
        m_grille[v]--;
      } else {
        m_grille[i]++;
      }
    }
  }
  void putBomb(int i){
    if(isBomb(i)){
      return;
    }
    m_grille[i] = -1;
    m_nb_bombs++;
    for(int v : voisins(i)){
      if(!isBomb(v)){
        m_grille[v]++;
      }
    }
  }
  
  int closestSafePosition(int i) const {
    while(i < m_grille.size() && isBomb(i)){
      i++;
    }
    return i;
  }
  int nb_safe_tiles() const {
    return get_size()*get_size() - get_nb_bombs();
  }
  int randomSafePosition() {
    std::uniform_int_distribution<std::mt19937::result_type> distrbute_on_safe(0, nb_safe_tiles() - 1 ); 
    int rand_pos = distrbute_on_safe(m_rand_gen);

    int result = closestSafePosition(0);
    for (int i = 0; i < rand_pos; ++i){
      result = closestSafePosition(result + 1);
    }
    return result;
  }
  
  void addBomb(){
    putBomb(randomSafePosition());
  }
  
  void addBombs(int nb){
    for (int i = 0; i < nb; ++i){
      addBomb();
    }
  }
};

std::ostream& operator<<(std::ostream& os , const Plateau& p){
  os << "Plateau : "<< p.get_nb_bombs() << " bombes de taille " 
    << p.get_size() << "x" << p.get_size() << std::endl;
  
  for (int y = 0; y < p.get_size(); ++y) {
    for (int x = 0; x < p.get_size(); ++x) {
      if(p.isBomb(x,y)){
        os << "| X " ; 
      } else {
        os << "| " << p.get(x,y) << " " ;
      }
    }
    os << "|" << std::endl;
  }
  return os;
}

class Game{
  enum class TileStatus{ Flaged, Revealed, Unrevealed };
  Plateau m_jeu;
  vector<TileStatus> m_mask;
public:
  Game(int size, int nb_bombs):m_jeu(size, nb_bombs), m_mask(size*size, TileStatus::Revealed) {}

  bool is_in_grille(int x, int y) const {
    return m_jeu.is_in_grille(x, y);
  }
  void reveal(int x, int y, bool safe = false){
    reveal(m_jeu.pos_to_index(x, y), safe);
  }
  void reveal(int i, bool safe = false){
    std::unordered_set<int> already_seen;
    reveal(i, already_seen, safe);
  }
  void reveal(int i, std::unordered_set<int>& already_seen, bool safe = false){
    if(flagged(i)){
      return;
    }
    m_mask[i] = TileStatus::Revealed;
    if(safe && m_jeu.isBomb(i)){
      m_jeu.addBomb();
      m_jeu.removeBomb(i);
    }
    if(m_jeu.get(i) != 0){
      return;
    }
    already_seen.insert(i);
    for(int v : m_jeu.voisins(i)){
      if(already_seen.count(v)){
        reveal(v, already_seen);
      }
    }
  }
  int get(int i) const {
    if(!revealed(i)){
      std::cerr << "Error : Tried to get the value of an unrevealed tile" << std::endl;
      exit(1);
    }
    return m_jeu.get(i);
  }
  int get(int x, int y) const {
    return get( m_jeu.pos_to_index(x, y) );
  }
  bool isBomb(int i) const {
    return get(i) == -1;
  }
  bool isBomb(int x, int y) const {
    return isBomb(m_jeu.pos_to_index(x, y));
  }
  int get_size() const {
    return m_jeu.get_size();
  }
  int get_nb_bombs() const {
    return m_jeu.get_nb_bombs();
  }
  bool flagged(int i) const {
    return m_mask[i] == TileStatus::Flaged;
  }
  bool flagged(int x, int y) const {
    return flagged( m_jeu.pos_to_index(x, y) );
  }
  bool revealed(int x, int y) const {
    return revealed( m_jeu.pos_to_index(x, y) );
  }
  bool revealed(int i) const {
    return m_mask[i] == TileStatus::Revealed;
  }
  void toggle_flag(int i){
    if(!revealed(i)){
      m_mask[i] = flagged(i) ? TileStatus::Unrevealed : TileStatus::Flaged ;
    }
  }
};

std::ostream& operator<<(std::ostream& os , const Game& g){
  os << "Game : "<< g.get_nb_bombs() << " bombes de taille " 
    << g.get_size() << "x" << g.get_size() << std::endl;
  
  for (int y = 0; y < g.get_size(); ++y) {
    for (int x = 0; x < g.get_size(); ++x) {
      if(!g.revealed(x, y)){
        os << "| " << (g.flagged(x,y) ? "#" : "-") <<" ";
      } else if(g.isBomb(x,y)){
        os << "| X " ; 
      } else {
        os << "| " << g.get(x,y) << " " ;
      }
    }
    os << "|" << std::endl;
  }
  return os;
}

class GamePlayer{
    Game m_game;
    bool is_first_move;
  public:
    GamePlayer(int size, int nb_bombs): m_game(size, nb_bombs), is_first_move(true) {}

    bool isMoveAllowed(int x, int y){
      return m_game.is_in_grille(x, y) && !m_game.flagged(x, y) && !m_game.revealed(x, y);
    }
    bool play(int x, int y){
      if(!isMoveAllowed(x, y)){
        return true;
      }
      m_game.reveal(x, y, is_first_move);
      is_first_move = false;
      return m_game.isBomb(x, y);
    }

    void show(Screen& s){
      auto [w, h] = s.getWindowSize();
      int s_w = w / m_game.get_size();
      int s_h = h / m_game.get_size();
      s.clear();
      s.replaceFont("font.ttf", 20);
      // std::cout << m_game;
      
      for (int y = 0; y < m_game.get_size(); ++y) {
        for (int x = 0; x < m_game.get_size(); ++x) {
          int x_pos = x*s_w;
          int y_pos = y*s_h;
          if(m_game.flagged(x, y)){
            s.drawImage("case_flag.png", x_pos, y_pos, s_w, s_h);
          } else if(!m_game.revealed(x, y)){
            s.drawImage("case_vide.png", x_pos, y_pos, s_w, s_h);
          } else if(m_game.isBomb(x, y)){
            s.drawImage("case_bombe.png", x_pos, y_pos, s_w, s_h);
          } else {
            s.drawImage("case_back.png", x_pos, y_pos, s_w, s_h);
            SDL_Color c = {.r = 200, .g=100, .b=0, .a=255};
            // std::cout << std::to_string(m_game.get(x, y)) << std::endl;
            s.drawString(std::to_string(m_game.get(x, y)), x_pos, y_pos, c);
          }
        }
      }
    }



};

// // Partie normale
// class Game {
//   // Regarder SDL
// public:
//   // Taille de la grille
//   int n;
//   // Nombre de bombes
//   int nbrbombes;

//   // Construit une grille de jeu démineur carré taille * taille, où il y aura
//   // nombrebombes bombes.
//   Game(int taille, int nombrebombes);

//   // Révèle la case (tx, ty), en révélant récursivement si la case est vide,
//   // toutes les cases autour. Renvoie l'état de la game après le coup :
//   // - 0 : état quelconque, la partie continue
//   // - -1 : la partie est terminé, une bombe se cachait sous la case
//   // - 1 : Le coup était inutile (déjà révélé, drapeau)
//   // - 2 : Le coup a mis fin a la partie, le joueur a révélé toutes les cases
//   // n'étant pas des bombes.
//   int Coup(int tx, int ty);

//   // Renvoie une copie de la partie identique dans le fond, mais séparé en terme
//   // de mémoire
//   Game Copy();

//   ~Game();

//   // Renvoie l'état visible de la partie
//   int **GetGame();

//   // Affiche dans la console l'état visible de la partie
//   void PrintGame();

//   // Joue dans la console une partie de démineur en taille 11*11 avec 10 bombes
//   static void GameConsole();

// private:
//   // Attention, ce constructeur présuppose que la partie a déjà commencé
//   // (minimum le coup de départ)
//   Game(int taille, int nombrebombes, int **tabreel, int **tabconnu,
//        int nbrcasesinconnues);

//   int **tabr;
//   int **tabc;
//   bool generated = false;
//   int nbrcasescaches;
//   int **generertableau(int x);
//   int **generergrille(int x0, int y0);
//   void printtableau(int **tab);
//   void freetableau(int **tab);
//   void reveler(int tx, int ty);
// };

// // Partie hypothétique, impossible d'y jouer ...

// class GameHypo {
// public:
//   int n;
//   int nbrbombes;

//   // Construit une grille hypothétique de démineur carré taille * taille, où il
//   // y aura nombrebombes bombes.
//   // Renvoie une position de départ sans aucune information
//   GameHypo(int taille, int nombrebombes);

//   // Un coup dans une game de ce type consiste en fait à déclarer une bombe
//   // Renvoie :
//   //  - 0 si la grille ne devient pas incohérente
//   //  - 1 s'il y a un problème
//   int Coup(int tx, int ty);

//   // Renvoie une copie de la partie identique dans le fond, mais séparé en terme
//   // de mémoire
//   GameHypo Copy();

//   ~GameHypo();

//   // Renvoie l'état visible de la partie
//   int **GetGame();

//   // Affiche dans la console l'état visible de la partie
//   void PrintGame();

// private:
//   // Essaye d'extraire un maximum d'informations de la grille connue
//   //(En fait je sais pas trop si c'est nécessaire mais bon)
//   // Renvoie :
//   //  - 1 si la grille est impossible
//   //  - 0 si une information a été déduite
//   //  - -1 si c'est un point fixe
//   int IterationCoup();

//   // Attention, ce constructeur présuppose que la partie a déjà commencé
//   // (minimum le coup de départ)
//   GameHypo(int taille, int nombrebombes, int **tabconnu, int nbrcasesinconnues);
//   int **tabc;
//   int nbrcasescaches;
//   int **generertableau(int x);
//   void printtableau(int **tab);
//   void freetableau(int **tab);
// };

