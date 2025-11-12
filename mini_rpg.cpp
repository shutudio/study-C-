/*
  파일명: mini_rpg.cpp
  빌드:  g++ -std=c++17 -O2 -Wall -Wextra mini_rpg.cpp -o mini_rpg
  실행:  ./mini_rpg

  개요:
  - 난이도 선택(Easy/Normal/Hard)
  - 전투: 공격 / 방어 / 포션 사용 / 도망
  - 적 처치 시 점수 획득, 라운드가 올라갈수록 난이도 상승
  - 하이스코어 파일 저장: minirpg_highscore.txt
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>
#include <cctype>

using namespace std;

struct Actor {
    string name;
    int hp{};
    int max_hp{};
    int atk_min{};
    int atk_max{};
    bool defending{false};
};

struct Player : Actor {
    int potions{3};
    long long score{0};
};

static mt19937& rng() {
    static mt19937 eng(static_cast<unsigned>(
        chrono::high_resolution_clock::now().time_since_epoch().count()));
    return eng;
}

int rint(int lo, int hi) {
    uniform_int_distribution<int> dist(lo, hi);
    return dist(rng());
}

string trim(const string& s){
    auto a = s.find_first_not_of(" \t\r\n");
    auto b = s.find_last_not_of(" \t\r\n");
    if(a==string::npos) return "";
    return s.substr(a, b-a+1);
}

int ask_menu(const vector<string>& items, const string& prompt="> ") {
    while (true) {
        for (size_t i = 0; i < items.size(); ++i) {
            cout << "  [" << (i+1) << "] " << items[i] << "\n";
        }
        cout << prompt;
        string line; getline(cin, line);
        line = trim(line);
        if (!line.empty() && all_of(line.begin(), line.end(), ::isdigit)) {
            int v = stoi(line);
            if (1 <= v && v <= (int)items.size()) return v;
        }
        cout << "올바른 번호를 입력해 주세요.\n";
    }
}

void press_enter() {
    cout << "\n(Enter를 눌러 계속)\n";
    string dummy; getline(cin, dummy);
}

long long load_highscore(const string& path) {
    ifstream fin(path);
    long long hs = 0;
    if (fin) fin >> hs;
    return hs;
}

void save_highscore(const string& path, long long hs) {
    ofstream fout(path, ios::trunc);
    if (fout) fout << hs;
}

void show_hud(const Player& me, const Actor& enemy, int round, const string& diffName) {
    cout << "\n=====================================\n";
    cout << " 난이도: " << diffName << " | 라운드 " << round << "\n";
    cout << "-------------------------------------\n";
    cout << " 나  HP: " << me.hp << "/" << me.max_hp
         << "  (포션 " << me.potions << "개)"
         << "  ATK " << me.atk_min << "~" << me.atk_max << "\n";
    cout << " 적  HP: " << enemy.hp << "/" << enemy.max_hp
         << "  ATK " << enemy.atk_min << "~" << enemy.atk_max << "\n";
    cout << " 점수: " << me.score << "\n";
    cout << "=====================================\n";
}

Actor make_enemy(int round, float scale, const vector<string>& names) {
    Actor e;
    e.name = names[rint(0, (int)names.size()-1)];
    int base = max(10, 12 + round*2);
    int hp = int(base * scale) + rint(0, round + 4);
    e.max_hp = e.hp = hp;
    e.atk_min = max(2, int(3*scale) + round/2);
    e.atk_max = e.atk_min + max(2, int(3*scale) + round/2 + 2);
    return e;
}

int player_attack(Player& me, Actor& enemy) {
    int dmg = rint(me.atk_min, me.atk_max);
    enemy.hp = max(0, enemy.hp - dmg);
    cout << ">> 당신의 공격! " << enemy.name << "에게 " << dmg << " 피해!\n";
    return dmg;
}

int enemy_attack(Actor& enemy, Player& me, int round) {
    // 간단 AI: 15% 확률로 강공격(라운드가 올라갈수록 위험)
    bool heavy = (rint(1,100) <= 15);
    int lo = enemy.atk_min;
    int hi = enemy.atk_max + (heavy ? max(1, round/2) : 0);
    int dmg = rint(lo, hi);

    if (me.defending) {
        dmg = int( round >= 8 ? dmg * 0.5 : dmg * 0.6 ); // 방어: 40~50% 피해 경감
        me.defending = false;
        cout << ">> 방어 성공! 피해가 감소되었습니다.\n";
    }
    me.hp = max(0, me.hp - dmg);
    cout << ">> " << enemy.name << "의 " << (heavy ? "강공격!" : "공격") << " 당신에게 "
         << dmg << " 피해!\n";
    return dmg;
}

void use_potion(Player& me) {
    if (me.potions <= 0) {
        cout << ">> 포션이 없습니다!\n";
        return;
    }
    int heal = rint(18, 28);
    int before = me.hp;
    me.hp = min(me.max_hp, me.hp + heal);
    me.potions--;
    cout << ">> 포션 사용! " << heal << " 회복 (" << before << " -> " << me.hp << ")\n";
}

bool try_run(Player& me, const Actor& enemy, int round) {
    // 라운드가 올라갈수록 도주 확률 감소
    int chance = max(15, 55 - round*4);
    bool ok = rint(1,100) <= chance;
    if (ok) {
        cout << ">> 도망에 성공했습니다. (점수 -5)\n";
        me.score = max(0LL, me.score - 5);
    } else {
        cout << ">> 도망 실패! (이번 턴 적에게 빈틈)\n";
    }
    return ok;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "==== 미니 텍스트 RPG ====\n";
    cout << "이름을 입력하세요: ";
    string name; getline(cin, name);
    if (trim(name).empty()) name = "모험가";

    // 난이도 선택
    cout << "\n난이도를 선택하세요.\n";
    int diff = ask_menu({"Easy", "Normal", "Hard"});
    string diffName = (diff==1? "Easy" : diff==2? "Normal" : "Hard");

    // 난이도 파라미터
    float enemy_scale = (diff==1? 0.9f : diff==2? 1.0f : 1.15f);
    int base_hp = (diff==1? 70 : diff==2? 60 : 55);
    int potions = (diff==1? 5 : diff==2? 3 : 2);
    int atk_min = (diff==1? 8 : diff==2? 7 : 6);
    int atk_max = (diff==1? 14 : diff==2? 13 : 12);
    double score_mul = (diff==1? 1.0 : diff==2? 1.25 : 1.5);

    Player me;
    me.name = name;
    me.max_hp = me.hp = base_hp;
    me.atk_min = atk_min;
    me.atk_max = atk_max;
    me.potions = potions;
    me.score = 0;

    const vector<string> enemy_names = {
        "슬라임", "고블린", "해골병사", "어둠늑대", "도적", "리자드맨", "미믹", "사막전갈"
    };

    const string hsPath = "minirpg_highscore.txt";
    long long best = load_highscore(hsPath);
    if (best > 0) cout << "현재 하이스코어: " << best << "\n";

    int round = 1;
    while (me.hp > 0) {
        Actor enemy = make_enemy(round, enemy_scale, enemy_names);
        cout << "\n--- 적이 나타났다! [" << enemy.name << "] ---\n";
        press_enter();

        // 전투 루프
        while (me.hp > 0 && enemy.hp > 0) {
            show_hud(me, enemy, round, diffName);
            int choice = ask_menu({"공격", "방어", "포션 사용", "도망"});

            bool enemy_turn = true;

            if (choice == 1) {                 // 공격
                player_attack(me, enemy);
                if (enemy.hp <= 0) break;
            } else if (choice == 2) {          // 방어
                me.defending = true;
                cout << ">> 방어 태세를 취합니다. (다음 적 공격 피해 감소)\n";
            } else if (choice == 3) {          // 포션
                use_potion(me);
            } else if (choice == 4) {          // 도망
                if (try_run(me, enemy, round)) {
                    enemy_turn = false; // 전투 종료
                    enemy.hp = -1;      // 도망 표식
                }
            }

            if (enemy_turn && enemy.hp > 0) {
                enemy_attack(enemy, me, round);
            }
        }

        if (me.hp <= 0) {
            cout << "\n당신은 쓰러졌습니다...\n";
            break;
        }

        if (enemy.hp <= 0) {
            // 보상: 점수, 소량 회복/드랍 확률
            int gain = int((15 + rint(0, 10) + round) * score_mul);
            me.score += gain;
            cout << "\n[" << enemy.name << "]을(를) 처치! +" << gain << " 점\n";

            // 드랍: 35% 포션, 35% 힐, 30% 없음
            int drop = rint(1,100);
            if (drop <= 35) {
                me.potions++;
                cout << ">> 포션을 획득했다! (총 " << me.potions << "개)\n";
            } else if (drop <= 70) {
                int heal = rint(6, 12);
                int before = me.hp;
                me.hp = min(me.max_hp, me.hp + heal);
                cout << ">> 전리품에서 회복약을 찾았다! +" << heal
                     << " HP (" << before << " -> " << me.hp << ")\n";
            } else {
                cout << ">> 전리품이 없다...\n";
            }
        } else {
            // 도망 성공
            cout << "\n당신은 무사히 전투에서 이탈했습니다.\n";
        }

        // 라운드 종료 후 선택
        cout << "\n다음 행동을 선택하세요.\n";
        int next = ask_menu({"다음 라운드로 간다", "안전하게 귀환(게임 종료)"});
        if (next == 2) break;

        // 난이도 스케일 상승, 플레이어 약간 강화(아주 소폭)
        round++;
        if (round % 3 == 0) {
            me.atk_max++;
        }
        if (round % 4 == 0) {
            me.max_hp += 2;
            me.hp = min(me.max_hp, me.hp + 2);
        }
    }

    cout << "\n=== 게임 종료 ===\n";
    cout << "플레이어: " << me.name << "\n";
    cout << "최종 점수: " << me.score << "\n";

    long long hs = load_highscore(hsPath);
    if (me.score > hs) {
        cout << "새 하이스코어 달성! (" << hs << " -> " << me.score << ")\n";
        save_highscore(hsPath, me.score);
    } else {
        cout << "현 하이스코어: " << hs << "\n";
    }

    cout << "플레이해줘서 고마워!\n";
    return 0;
}
