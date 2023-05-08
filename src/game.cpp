#include "game.h"

Plateau::Plateau(int size):
	m_nb_bombs(0), 
	m_size(size), 
	m_grille(m_size*m_size, 0),
	m_dev(),
	m_rand_gen(m_dev())  {
}
Plateau::Plateau(int size, int nb_bombs):Plateau(size){ addBombs(nb_bombs); }

int Plateau::get_size() const {
	return m_size;
}
int Plateau::get_nb_bombs() const {
	return m_nb_bombs;
}
int Plateau::get(int i) const {
	return m_grille[i];
}
int Plateau::get(int x, int y) const {
	return get(pos_to_index(x, y));
}
void Plateau::reset(){
	m_nb_bombs = 0;
	for (size_t i = 0; i < m_grille.size(); ++i){
		m_grille[i] = 0;
	}
}

int Plateau::pos_to_index(int x, int y) const {
	return x + m_size*y;
}

bool Plateau::is_in_grille(int x, int y) const {
	return (x >= 0)&&(x < m_size)&&(y >= 0)&&(y < m_size);
}

std::vector<int> Plateau::voisins(int x, int y) const {
	std::vector<int> result;
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

std::vector<int> Plateau::voisins(int i) const {
	return voisins( i%m_size, i/m_size );
}

bool Plateau::isBomb(int i) const {
	return m_grille[i] == -1;
}
bool Plateau::isBomb(int x, int y) const {
	return isBomb(pos_to_index(x, y));
}
void Plateau::removeBomb(int i){
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
void Plateau::putBomb(int i){
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

int Plateau::closestSafePosition(size_t i) const {
	while(i < m_grille.size() && isBomb(i)){
		i++;
	}
	return i;
}
int Plateau::nb_safe_tiles() const {
	return get_size()*get_size() - get_nb_bombs();
}
int Plateau::randomSafePosition() {
	std::uniform_int_distribution<std::mt19937::result_type> distrbute_on_safe(0, nb_safe_tiles() - 1 ); 
	int rand_pos = distrbute_on_safe(m_rand_gen);

	int result = closestSafePosition(0);
	for (int i = 0; i < rand_pos; ++i){
		result = closestSafePosition(result + 1);
	}
	return result;
}

void Plateau::addBomb(){
	putBomb(randomSafePosition());
}

void Plateau::addBombs(int nb){
	for (int i = 0; i < nb; ++i){
		addBomb();
	}
}

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

Game::Game(int size, int nb_bombs):m_jeu(size, nb_bombs), m_mask(size*size, TileStatus::Unrevealed) {
	// for (int i = 0; i < size*size; ++i){
	// 	m_modified.push_back(i);
	// }
}
// void Game::reset_modified(){
// 	m_modified.clear();
// 	for (int i = 0; i < get_size()*get_size(); ++i){
// 		m_modified.push_back(i);
// 	}	
// }
// void Game::clear_modified(){
// 	m_modified.clear();
// }
// const std::vector<int>& Game::get_modified() const {
// 	return m_modified;
// }
bool Game::is_in_grille(int x, int y) const {
	return m_jeu.is_in_grille(x, y);
}
void Game::reveal(int x, int y, bool safe){
	reveal(m_jeu.pos_to_index(x, y), safe);
}
void Game::reveal(int i, bool safe){
	std::unordered_set<int> already_seen;
	reveal(i, already_seen, safe);
}
void Game::reveal(int i, std::unordered_set<int>& already_seen, bool safe){
	if(flagged(i)){
		return;
	}
	// m_modified.push_back(i);
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
		if(!already_seen.count(v)){
			reveal(v, already_seen);
		}
	}
}
int Game::get(int i) const {
if(!revealed(i)){
	std::cerr << "Error : Tried to get the value of an unrevealed tile" << std::endl;
	exit(1);
}
	return m_jeu.get(i);
}
int Game::get(int x, int y) const {
	return get( m_jeu.pos_to_index(x, y) );
}
bool Game::isBomb(int i) const {
	return get(i) == -1;
}
bool Game::isBomb(int x, int y) const {
	return isBomb(m_jeu.pos_to_index(x, y));
}
int Game::get_size() const {
	return m_jeu.get_size();
}
int Game::get_nb_bombs() const {
	return m_jeu.get_nb_bombs();
}
bool Game::flagged(int i) const {
	return m_mask[i] == TileStatus::Flaged;
}
bool Game::flagged(int x, int y) const {
	return flagged( m_jeu.pos_to_index(x, y) );
}
bool Game::revealed(int x, int y) const {
	return revealed( m_jeu.pos_to_index(x, y) );
}
bool Game::revealed(int i) const {
	return m_mask[i] == TileStatus::Revealed;
}
void Game::toggle_flag(int i){
	if(!revealed(i)){
		// m_modified.push_back(i);
		m_mask[i] = flagged(i) ? TileStatus::Unrevealed : TileStatus::Flaged ;
	}
}
void Game::toggle_flag(int x, int y) {
	return toggle_flag( m_jeu.pos_to_index(x, y) );
}


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
