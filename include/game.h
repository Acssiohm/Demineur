#pragma once
#include <unordered_set>
#include <vector>
#include <iostream>
#include <random>

class Plateau {
  int m_nb_bombs;
  int m_size;
  std::vector<int> m_grille;
  std::random_device m_dev;
  std::mt19937 m_rand_gen;

public:
  Plateau(int size);
  Plateau(int size, int nb_bombs);

  int get_size() const ;
  int get_nb_bombs() const ;

  int pos_to_index(int x, int y) const ;
  
  bool isBomb(int i) const;
  bool isBomb(int x, int y) const ;
  int get(int i) const;
  int get(int x, int y) const ;
  bool is_in_grille(int x, int y) const ;
  int nb_safe_tiles() const;

  std::vector<int> voisins(int x, int y) const ;
  std::vector<int> voisins(int i) const;
  
  int closestSafePosition(int i) const ;
  int randomSafePosition() ;
  
  void putBomb(int i);
  void removeBomb(int i);
  void addBomb();
  void addBombs(int nb);
  
  void reset();
};

std::ostream& operator<<(std::ostream& os , const Plateau& p);

class Game{
  enum class TileStatus{ Flaged, Revealed, Unrevealed };
  Plateau m_jeu;
  std::vector<TileStatus> m_mask;
  // std::vector<int> m_modified;

public:
  Game(int size, int nb_bombs);
  
  // void clear_modified();
  // void reset_modified();
  const std::vector<int>& get_modified() const ;

  void reveal(int x, int y, bool safe = false);
  void reveal(int i, bool safe = false);
  void reveal(int i, std::unordered_set<int>& already_seen, bool safe = false);
  
  int get_size() const;
  int get_nb_bombs() const;
  bool is_in_grille(int x, int y) const ;

  int get(int i) const ;
  int get(int x, int y) const ;
  
  bool isBomb(int i) const ;
  bool isBomb(int x, int y) const ;
  
  bool flagged(int i) const ;
  bool flagged(int x, int y) const ;
  
  bool revealed(int x, int y) const;
  bool revealed(int i) const;
  
  void toggle_flag(int i);
  void toggle_flag(int x, int y);
};

std::ostream& operator<<(std::ostream& os , const Game& g);
