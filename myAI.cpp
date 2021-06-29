#include <iostream>
#include <fstream>
//#include <string>
//#include <sstream>
#include <array>
#include <vector>
//#include <cassert>
#include <cstdlib>
#include <ctime>

#define max(a, b) a>b?a:b
#define min(a, b) a<b?a:b

struct Point {
    int x, y;
    Point() : Point(0, 0) {}
    Point(float x, float y) : x(x), y(y) {}
    bool operator==(const Point& rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const Point& rhs) const {
        return !operator==(rhs);
    }
    Point operator+(const Point& rhs) const {
        return Point(x + rhs.x, y + rhs.y);
    }
    Point operator-(const Point& rhs) const {
        return Point(x - rhs.x, y - rhs.y);
    }
};

class OthelloBoard {
public:
    enum SPOT_STATE {
        EMPTY = 0,
        BLACK = 1,
        WHITE = 2
    };
    static const int SIZE = 8;
    const std::array<Point, 8> directions{{
        Point(-1, -1), Point(-1, 0), Point(-1, 1),
        Point(0, -1), /*{0, 0}, */Point(0, 1),
        Point(1, -1), Point(1, 0), Point(1, 1)
    }};
    std::array<std::array<int, SIZE>, SIZE> board;
    //int board[8][8];
    std::vector<Point> Next_valid_spots;
    std::array<int, 3> disc_count;
    int cur_player;
    bool done;
    int winner;
private:
    int get_next_player(int player) const {
        return 3 - player;
    }
    bool is_spot_on_board(Point p) const {
        return 0 <= p.x && p.x < SIZE && 0 <= p.y && p.y < SIZE;
    }
    int get_disc(Point p) const {
        if((p.x>=0&&p.x<8)||(p.y>=0&&p.y<8))return board[p.x][p.y];
        else return -1;//????????????????
    }
    void set_disc(Point p, int disc) {
        board[p.x][p.y] = disc;
    }
    bool is_disc_at(Point p, int disc) const {
        if (!is_spot_on_board(p))
            return false;
        if (get_disc(p) != disc)
            return false;
        return true;
    }
    bool is_spot_valid(Point center) const {
        if (get_disc(center) != EMPTY)
            return false;
        for (Point dir: directions) {
            // Move along the direction while testing.
            Point p = center + dir;
            if (!is_disc_at(p, get_next_player(cur_player)))
                continue;
            p = p + dir;
            while (is_spot_on_board(p) && get_disc(p) != EMPTY) {
                if (is_disc_at(p, cur_player))
                    return true;
                p = p + dir;
            }
        }
        return false;
    }
    void flip_discs(Point center) {
        for (Point dir: directions) {
            // Move along the direction while testing.
            Point p = center + dir;
            if (!is_disc_at(p, get_next_player(cur_player)))
                continue;
            std::vector<Point> discs({p});
            p = p + dir;
            while (is_spot_on_board(p) && get_disc(p) != EMPTY) {
                if (is_disc_at(p, cur_player)) {
                    for (Point s: discs) {
                        set_disc(s, cur_player);
                    }
                    disc_count[cur_player] += discs.size();
                    disc_count[get_next_player(cur_player)] -= discs.size();
                    break;
                }
                discs.push_back(p);
                p = p + dir;
            }
        }
    }
public:
    OthelloBoard() {
        reset();
    }
    void reset() {
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                board[i][j] = EMPTY;
            }
        }
        board[3][4] = board[4][3] = BLACK;
        board[3][3] = board[4][4] = WHITE;
        cur_player = BLACK;
        disc_count[EMPTY] = 8*8-4;
        disc_count[BLACK] = 2;
        disc_count[WHITE] = 2;
        Next_valid_spots = get_valid_spots();
        done = false;
        winner = -1;
    }
    std::vector<Point> get_valid_spots() const {
        std::vector<Point> valid_spots;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                Point p = Point(i, j);
                if (board[i][j] != EMPTY)
                    continue;
                if (is_spot_valid(p))
                    valid_spots.push_back(p);
            }
        }
        return valid_spots;
    }
    bool put_disc(Point p) {
        if(!is_spot_valid(p)) {
            winner = get_next_player(cur_player);
            done = true;
            return false;
        }
        set_disc(p, cur_player);
        disc_count[cur_player]++;
        disc_count[EMPTY]--;
        flip_discs(p);
        // Give control to the other player.
        cur_player = get_next_player(cur_player);
        Next_valid_spots = get_valid_spots();
        // Check Win
        if (Next_valid_spots.size() == 0) {
            cur_player = get_next_player(cur_player);
            Next_valid_spots = get_valid_spots();
            if (Next_valid_spots.size() == 0) {
                // Game ends
                done = true;
                int white_discs = disc_count[WHITE];
                int black_discs = disc_count[BLACK];
                if (white_discs == black_discs) winner = EMPTY;
                else if (black_discs > white_discs) winner = BLACK;
                else winner = WHITE;
            }
        }
        return true;
    }
};

int player;
int ansx,ansy;
int value;
const int SIZE = 8;
std::array<std::array<int, SIZE>, SIZE> brd;
std::vector<Point> next_valid_spots;

const std::array<Point, 8> directions{{
    Point(-1, -1), Point(-1, 0), Point(-1, 1),
    Point(0, -1), /*{0, 0}, */Point(0, 1),
    Point(1, -1), Point(1, 0), Point(1, 1)
}};

int score[8][8] = {
    {100, -25, 10, 4, 4, 10, -25, 100},
    {-25, -40,  3, 1, 1,  3, -40, -25},
    { 10,   2,  5, 3, 3,  5,   2,  10},
    {  7,   1,  3, 1, 1,  3,   1,   7},
    {  7,   1,  3, 1, 1,  3,   1,   7},
    { 10,   2,  5, 3, 3,  5,   2,  10},
    {-25, -40,  3, 1, 1,  3, -40, -25},
    {100, -25, 10, 4, 4, 10, -25, 100} };//數值反映Disc的相對重要程度

void read_board(std::ifstream& fin) { //offer the 8*8 array: board [i][j]
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> brd[i][j];
        }
    }
}

void read_valid_spots(std::ifstream& fin) { //offer valid moves(in vector-form): next_valid_spots
    int n_valid_spots;
    fin >> n_valid_spots;
    int x, y;
    for (int i = 0; i < n_valid_spots; i++) {
        fin >> x >> y;
        next_valid_spots.push_back({static_cast<float>(x), static_cast<float>(y)});
    }
}

int alphabeta(OthelloBoard oboard, int depth, int alpha, int beta, int curPlayer);

int count_score(std::array<std::array<int, SIZE>, SIZE> Board){
    int val = 0;
    for(int i=0;i<SIZE;i++)for(int j=0;j<SIZE;j++){
        if(Board[i][j]==player) val += score[i][j];//player's disc(BLACK)//如果後手也要就改這裡
        else if(Board[i][j]!=0) val -= score[i][j];//opponent's(WHITE)
        //else =0, just do nothing
    }
    return val;
}
int alphabeta(OthelloBoard oboard, Point spot, int depth, int alpha, int beta, int curPlayer){
    OthelloBoard OB = oboard;
    //if(spot.x==3&&spot.y==4)OB.Next_valid_spots = OB.get_valid_spots();
    OB.put_disc(spot);//previous round's
    if (depth == 0 || OB.done) return count_score(OB.board);
    
    else if (curPlayer==player){
        int val = -87878787;
        if(OB.Next_valid_spots.size()==0){//player this round cant move
            int tmp = alphabeta(OB, depth-1, alpha, beta, 3-OB.cur_player);
            val = max(val, tmp);
            alpha = max(alpha, val);
        }
        else for(auto i: OB.Next_valid_spots){
            int tmp = alphabeta(OB, i, depth-1, alpha, beta, 3-OB.cur_player);
            val = max(val, tmp);
            if (val >= beta) break;//....
            alpha = max(alpha, val);
        }
        return val;
    }
    else{
        int val = 87878787;
        if(OB.Next_valid_spots.size()==0){//player this round cant move
            int tmp = alphabeta(OB, depth-1, alpha, beta, 3-OB.cur_player);
            val = min(val, tmp);
            beta = min(beta, val);
        }
        else for(auto i: OB.Next_valid_spots){
            int tmp = alphabeta(OB, i, depth-1, alpha, beta, 3-OB.cur_player);
            val = min(val, tmp);
            if (val <= alpha) break;//....
            beta = min(beta, val);
        }
        return val;
    }
}
int alphabeta(OthelloBoard oboard, int depth, int alpha, int beta, int curPlayer){
    OthelloBoard OB = oboard;
    OB.Next_valid_spots = OB.get_valid_spots();//previous round's
    if (depth == 0 || OB.done) return count_score(OB.board);
    
    if (curPlayer==player){
        int val = -87878787;
        if(OB.Next_valid_spots.size()==0){//player this round cant move
            int tmp = alphabeta(OB, depth-1, alpha, beta, 3-OB.cur_player);
            val = max(val, tmp);
            alpha = max(alpha, val);
        }
        else for(auto i: OB.Next_valid_spots){
            int tmp = alphabeta(OB, i, depth-1, alpha, beta, 3-OB.cur_player);
            val = max(val, tmp);
            if (val >= beta) break;//....
            alpha = max(alpha, val);
        }
        return val;
    }
    else{
        int val = 87878787;
        if(OB.Next_valid_spots.size()==0){//player this round cant move
            int tmp = alphabeta(OB, depth-1, alpha, beta, 3-OB.cur_player);
            val = min(val, tmp);
            beta = min(beta, val);
        }
        else for(auto i: OB.Next_valid_spots){
            int tmp = alphabeta(OB, i, depth-1, alpha, beta, 3-OB.cur_player);
            val = min(val, tmp);
            if (val <= alpha) break;//....
            beta = min(beta, val);
        }
        return val;
    }
}
void write_valid_spot(std::ofstream& fout) { //output next spot
    int n_valid_spots = next_valid_spots.size();
    srand(time(NULL));
    // Choose random spot. (Not random uniform here)
    int index = (rand() % n_valid_spots);//find out index
    Point P = next_valid_spots[index];
    //if(find_next_idx() != -1)int index = find_next_idx();
    OthelloBoard B;
    for(int i=0;i<8;i++)for(int j=0;j<8;j++) B.board[i][j]=brd[i][j];
    B.cur_player = player;
    B.Next_valid_spots = next_valid_spots;
    int ans = -87878787;
    for(auto s: next_valid_spots){
        int tmp = alphabeta(B, s, 6, -87878787, 87878787, player);
        if(tmp>ans){
            ans=tmp;
            P.x=s.x;
            P.y=s.y;
        }
    }
    // Remember to flush the output to ensure the last action is written to file.
    fout << P.x << " " << P.y << std::endl;
    fout.flush();
}

int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    read_board(fin);
    read_valid_spots(fin);
    write_valid_spot(fout);
    fin.close();
    fout.close();
    return 0;
}
