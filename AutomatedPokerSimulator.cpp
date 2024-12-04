// #include <iostream>
// #include <vector>
// #include <algorithm>
// #include <random>
// #include <chrono>
// #include <unordered_set>
// #include <iomanip>
// #include <sstream>
// #include <map>
// #include <unordered_map>
// #include <fstream>
//
// using namespace std;
//
// // Enumerations for Suit and Rank
// enum Suit { HEARTS, DIAMONDS, CLUBS, SPADES };
// enum Rank {
//     TWO = 2, THREE, FOUR, FIVE, SIX, SEVEN,
//     EIGHT, NINE, TEN, JACK, QUEEN, KING, ACE
// };
//
// // Structure to represent a Card
// struct Card {
//     Suit suit;
//     Rank rank;
//
//     Card(Suit s, Rank r) : suit(s), rank(r) {}
//
//     // Overload == operator for comparison
//     bool operator==(const Card& other) const {
//         return (suit == other.suit) && (rank == other.rank);
//     }
// };
//
// // Function to convert Card object to string
// string cardToString(const Card& card) {
//     string rankStr;
//     switch (card.rank) {
//     case TWO: rankStr = "2"; break;
//     case THREE: rankStr = "3"; break;
//     case FOUR: rankStr = "4"; break;
//     case FIVE: rankStr = "5"; break;
//     case SIX: rankStr = "6"; break;
//     case SEVEN: rankStr = "7"; break;
//     case EIGHT: rankStr = "8"; break;
//     case NINE: rankStr = "9"; break;
//     case TEN: rankStr = "10"; break;
//     case JACK: rankStr = "J"; break;
//     case QUEEN: rankStr = "Q"; break;
//     case KING: rankStr = "K"; break;
//     case ACE: rankStr = "A"; break;
//     }
//
//     string suitStr;
//     switch (card.suit) {
//     case HEARTS: suitStr = "h"; break;
//     case DIAMONDS: suitStr = "d"; break;
//     case CLUBS: suitStr = "c"; break;
//     case SPADES: suitStr = "s"; break;
//     }
//
//     return rankStr + suitStr;
// }
//
// // Structure to represent evaluated hand value
// struct HandValue {
//     int category; // 1 to 9
//     vector<int> tiebreakers; // For comparing hands within the same category
//
//     bool operator<(const HandValue& other) const {
//         if (category != other.category)
//             return category < other.category;
//         for (size_t i = 0; i < tiebreakers.size(); ++i) {
//             if (i >= other.tiebreakers.size())
//                 return false;
//             if (tiebreakers[i] != other.tiebreakers[i])
//                 return tiebreakers[i] < other.tiebreakers[i];
//         }
//         return false;
//     }
//
//     bool operator>(const HandValue& other) const {
//         return other < *this;
//     }
//
//     bool operator==(const HandValue& other) const {
//         return (category == other.category) && (tiebreakers == other.tiebreakers);
//     }
// };
//
// // Deck class to manage cards
// class Deck {
// public:
//     vector<Card> cards;
//
//     Deck(const vector<Card>& excludedCards) {
//         // Initialize full deck
//         for (int s = HEARTS; s <= SPADES; ++s) {
//             for (int r = TWO; r <= ACE; ++r) {
//                 Card card(static_cast<Suit>(s), static_cast<Rank>(r));
//                 // Check if card is excluded
//                 bool excluded = false;
//                 for (const auto& ec : excludedCards) {
//                     if (card == ec) {
//                         excluded = true;
//                         break;
//                     }
//                 }
//                 if (!excluded)
//                     cards.push_back(card);
//             }
//         }
//     }
//
//     void shuffleDeck() {
//         random_device rd;
//         mt19937 g(rd());
//         shuffle(cards.begin(), cards.end(), g);
//     }
// };
//
// // HandEvaluator class to evaluate poker hands
// class HandEvaluator {
// public:
//     // Map-based hand evaluation
//     HandValue evaluateHandMap(const vector<Card>& hand) {
//         // Use std::map for counting
//         map<int, int> rankCount;
//         map<Suit, int> suitCount;
//         return evaluateHandGeneric(hand, rankCount, suitCount);
//     }
//
//     // Hash table-based hand evaluation
//     HandValue evaluateHandHash(const vector<Card>& hand) {
//         // Use std::unordered_map for counting
//         unordered_map<int, int> rankCount;
//         unordered_map<Suit, int> suitCount;
//         return evaluateHandGeneric(hand, rankCount, suitCount);
//     }
//
// private:
//     template<typename RankMap, typename SuitMap>
//     HandValue evaluateHandGeneric(const vector<Card>& hand, RankMap& rankCount, SuitMap& suitCount) {
//         // Sort the hand by rank descending
//         vector<Card> sortedHand = hand;
//         sort(sortedHand.begin(), sortedHand.end(), [&](const Card& a, const Card& b) {
//             return a.rank > b.rank;
//             });
//
//         vector<int> ranks;
//         for (const auto& card : sortedHand) {
//             rankCount[card.rank]++;
//             suitCount[card.suit]++;
//             ranks.push_back(card.rank);
//         }
//
//         bool isFlush = false;
//         Suit flushSuit;
//         for (const auto& sc : suitCount) {
//             if (sc.second >= 5) {
//                 isFlush = true;
//                 flushSuit = sc.first;
//                 break;
//             }
//         }
//
//         // Extract ranks for straight
//         vector<int> uniqueRanks;
//         for (const auto& rc : rankCount) {
//             uniqueRanks.push_back(rc.first);
//         }
//         sort(uniqueRanks.begin(), uniqueRanks.end(), greater<int>());
//
//         // Check for straight (including Ace-low)
//         bool isStraight = false;
//         int highStraight = 0;
//         if (uniqueRanks.size() >= 5) {
//             for (size_t i = 0; i <= uniqueRanks.size() - 5; ++i) {
//                 bool consecutive = true;
//                 for (size_t j = 0; j < 4; ++j) {
//                     if (uniqueRanks[i + j] - 1 != uniqueRanks[i + j + 1]) {
//                         consecutive = false;
//                         break;
//                     }
//                 }
//                 if (consecutive) {
//                     isStraight = true;
//                     highStraight = uniqueRanks[i];
//                     break;
//                 }
//             }
//             // Check for Ace-low straight
//             if (!isStraight) {
//                 // A, 2, 3, 4, 5
//                 if (find(uniqueRanks.begin(), uniqueRanks.end(), ACE) != uniqueRanks.end() &&
//                     find(uniqueRanks.begin(), uniqueRanks.end(), TWO) != uniqueRanks.end() &&
//                     find(uniqueRanks.begin(), uniqueRanks.end(), THREE) != uniqueRanks.end() &&
//                     find(uniqueRanks.begin(), uniqueRanks.end(), FOUR) != uniqueRanks.end() &&
//                     find(uniqueRanks.begin(), uniqueRanks.end(), FIVE) != uniqueRanks.end()) {
//                     isStraight = true;
//                     highStraight = FIVE;
//                 }
//             }
//         }
//
//         // Check for straight flush
//         bool isStraightFlush = false;
//         int highStraightFlush = 0;
//         if (isFlush) {
//             // Extract cards of the flush suit
//             vector<Card> flushCards;
//             for (const auto& card : sortedHand) {
//                 if (card.suit == flushSuit)
//                     flushCards.push_back(card);
//             }
//             // Check for straight in flushCards
//             vector<int> flushRanks;
//             unordered_map<int, int> flushRankCount;
//             for (const auto& card : flushCards) {
//                 flushRankCount[card.rank]++;
//                 flushRanks.push_back(card.rank);
//             }
//             sort(flushRanks.begin(), flushRanks.end(), greater<int>());
//             // Remove duplicates
//             flushRanks.erase(unique(flushRanks.begin(), flushRanks.end()), flushRanks.end());
//
//             if (flushRanks.size() >= 5) {
//                 for (size_t i = 0; i <= flushRanks.size() - 5; ++i) {
//                     bool consecutive = true;
//                     for (size_t j = 0; j < 4; ++j) {
//                         if (flushRanks[i + j] - 1 != flushRanks[i + j + 1]) {
//                             consecutive = false;
//                             break;
//                         }
//                     }
//                     if (consecutive) {
//                         isStraightFlush = true;
//                         highStraightFlush = flushRanks[i];
//                         break;
//                     }
//                 }
//                 // Check for Ace-low straight flush
//                 if (!isStraightFlush) {
//                     if (find(flushRanks.begin(), flushRanks.end(), ACE) != flushRanks.end() &&
//                         find(flushRanks.begin(), flushRanks.end(), TWO) != flushRanks.end() &&
//                         find(flushRanks.begin(), flushRanks.end(), THREE) != flushRanks.end() &&
//                         find(flushRanks.begin(), flushRanks.end(), FOUR) != flushRanks.end() &&
//                         find(flushRanks.begin(), flushRanks.end(), FIVE) != flushRanks.end()) {
//                         isStraightFlush = true;
//                         highStraightFlush = FIVE;
//                     }
//                 }
//             }
//         }
//
//         // Determine hand category and tiebreakers
//         HandValue hv;
//         if (isStraightFlush) {
//             hv.category = 9; // Straight Flush
//             hv.tiebreakers.push_back(highStraightFlush);
//         }
//         else {
//             // Check for Four of a Kind
//             bool fourKind = false;
//             int fourRank = 0;
//             for (const auto& rc : rankCount) {
//                 if (rc.second == 4) {
//                     fourKind = true;
//                     fourRank = rc.first;
//                     break;
//                 }
//             }
//             if (fourKind) {
//                 hv.category = 8; // Four of a Kind
//                 hv.tiebreakers.push_back(fourRank);
//                 // Add highest kicker
//                 for (const auto& card : sortedHand) {
//                     if (card.rank != fourRank) {
//                         hv.tiebreakers.push_back(card.rank);
//                         break;
//                     }
//                 }
//             }
//             else {
//                 // Check for Full House
//                 bool threeKind = false;
//                 int threeRank = 0;
//                 vector<int> pairs;
//                 for (const auto& rc : rankCount) {
//                     if (rc.second == 3) {
//                         if (!threeKind || rc.first > threeRank) {
//                             threeKind = true;
//                             threeRank = rc.first;
//                         }
//                     }
//                     else if (rc.second == 2) {
//                         pairs.push_back(rc.first);
//                     }
//                 }
//                 if (threeKind && (pairs.size() >= 1 || (threeRank && rankCount.size() >= 2))) {
//                     hv.category = 7; // Full House
//                     hv.tiebreakers.push_back(threeRank);
//                     // Find the highest pair
//                     int highestPair = 0;
//                     for (const auto& pr : pairs) {
//                         if (pr > highestPair) highestPair = pr;
//                     }
//                     // If no pairs, find second three of a kind
//                     if (highestPair == 0) {
//                         for (const auto& rc : rankCount) {
//                             if (rc.second == 3 && rc.first != threeRank) {
//                                 if (rc.first > highestPair) highestPair = rc.first;
//                             }
//                         }
//                     }
//                     hv.tiebreakers.push_back(highestPair);
//                 }
//                 else {
//                     // Check for Flush
//                     if (isFlush) {
//                         hv.category = 6; // Flush
//                         // Add top five cards of flush
//                         int count = 0;
//                         for (const auto& card : sortedHand) {
//                             if (card.suit == flushSuit) {
//                                 hv.tiebreakers.push_back(card.rank);
//                                 count++;
//                                 if (count == 5) break;
//                             }
//                         }
//                     }
//                     else {
//                         // Check for Straight
//                         if (isStraight) {
//                             hv.category = 5; // Straight
//                             hv.tiebreakers.push_back(highStraight);
//                         }
//                         else {
//                             // Check for Three of a Kind
//                             if (threeKind) {
//                                 hv.category = 4; // Three of a Kind
//                                 hv.tiebreakers.push_back(threeRank);
//                                 // Add two highest kickers
//                                 int kickers = 0;
//                                 for (const auto& card : sortedHand) {
//                                     if (card.rank != threeRank) {
//                                         hv.tiebreakers.push_back(card.rank);
//                                         kickers++;
//                                         if (kickers == 2) break;
//                                     }
//                                 }
//                             }
//                             else {
//                                 // Check for Two Pair
//                                 vector<int> pairsFound;
//                                 for (const auto& rc : rankCount) {
//                                     if (rc.second == 2) {
//                                         pairsFound.push_back(rc.first);
//                                     }
//                                 }
//                                 if (pairsFound.size() >= 2) {
//                                     hv.category = 3; // Two Pair
//                                     sort(pairsFound.begin(), pairsFound.end(), greater<int>());
//                                     hv.tiebreakers.push_back(pairsFound[0]);
//                                     hv.tiebreakers.push_back(pairsFound[1]);
//                                     // Add highest kicker
//                                     for (const auto& card : sortedHand) {
//                                         if (card.rank != pairsFound[0] && card.rank != pairsFound[1]) {
//                                             hv.tiebreakers.push_back(card.rank);
//                                             break;
//                                         }
//                                     }
//                                 }
//                                 else {
//                                     // Check for One Pair
//                                     if (pairsFound.size() == 1) {
//                                         hv.category = 2; // One Pair
//                                         hv.tiebreakers.push_back(pairsFound[0]);
//                                         // Add three highest kickers
//                                         int kickers = 0;
//                                         for (const auto& card : sortedHand) {
//                                             if (card.rank != pairsFound[0]) {
//                                                 hv.tiebreakers.push_back(card.rank);
//                                                 kickers++;
//                                                 if (kickers == 3) break;
//                                             }
//                                         }
//                                     }
//                                     else {
//                                         // High Card
//                                         hv.category = 1; // High Card
//                                         // Add top five cards
//                                         for (int i = 0; i < 5 && i < sortedHand.size(); ++i) {
//                                             hv.tiebreakers.push_back(sortedHand[i].rank);
//                                         }
//                                     }
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//
//         return hv;
//     }
// };
//
// // Simulator class to perform Monte Carlo simulations
// class Simulator {
// private:
//     vector<Card> player1Hand;
//     vector<Card> player2Hand;
//     vector<Card> communityCards;
//     string gameStage;
//     HandEvaluator evaluator;
//
// public:
//     Simulator(const vector<Card>& p1Hand, const vector<Card>& p2Hand, const string& stage, const vector<Card>& commCards)
//         : player1Hand(p1Hand), player2Hand(p2Hand), gameStage(stage), communityCards(commCards) {}
//
//     // Function to get all used cards (players' hands and community cards)
//     vector<Card> getAllUsedCards() const {
//         vector<Card> used = player1Hand;
//         used.insert(used.end(), player2Hand.begin(), player2Hand.end());
//         used.insert(used.end(), communityCards.begin(), communityCards.end());
//         return used;
//     }
//
//     // Function to determine how many community cards are needed based on game stage
//     int neededCommunityCards() const {
//         if (gameStage == "preflop") return 5;
//         else if (gameStage == "flop") return 2;
//         else if (gameStage == "turn") return 1;
//         else if (gameStage == "river") return 0;
//         else return 0;
//     }
//
//     // Function to run map-based simulation
//     void runSimulationMap(int trials, double& p1Win, double& p2Win, double& tie, long long& execTime) {
//         int p1Wins = 0, p2Wins = 0, ties = 0;
//         auto startTime = chrono::high_resolution_clock::now();
//
//         for (int i = 0; i < trials; ++i) {
//             // Initialize deck
//             Deck deck(getAllUsedCards());
//             deck.shuffleDeck();
//
//             // Deal needed community cards
//             vector<Card> simCommunity = communityCards;
//             int cardsToDeal = neededCommunityCards();
//             for (int c = 0; c < cardsToDeal; ++c) {
//                 if (deck.cards.empty()) break; // Safety check
//                 simCommunity.push_back(deck.cards.back());
//                 deck.cards.pop_back();
//             }
//
//             // Evaluate hands using map
//             vector<Card> p1Total = player1Hand;
//             p1Total.insert(p1Total.end(), simCommunity.begin(), simCommunity.end());
//             vector<Card> p2Total = player2Hand;
//             p2Total.insert(p2Total.end(), simCommunity.begin(), simCommunity.end());
//
//             HandValue hv1 = evaluator.evaluateHandMap(p1Total);
//             HandValue hv2 = evaluator.evaluateHandMap(p2Total);
//
//             // Compare hands
//             if (hv1 > hv2) p1Wins++;
//             else if (hv2 > hv1) p2Wins++;
//             else ties++;
//         }
//
//         auto endTime = chrono::high_resolution_clock::now();
//         execTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
//
//         p1Win = (p1Wins / static_cast<double>(trials)) * 100.0;
//         p2Win = (p2Wins / static_cast<double>(trials)) * 100.0;
//         tie = (ties / static_cast<double>(trials)) * 100.0;
//     }
//
//     // Function to run hash table-based simulation
//     void runSimulationHash(int trials, double& p1Win, double& p2Win, double& tie, long long& execTime) {
//         int p1Wins = 0, p2Wins = 0, ties = 0;
//         auto startTime = chrono::high_resolution_clock::now();
//
//         for (int i = 0; i < trials; ++i) {
//             // Initialize deck
//             Deck deck(getAllUsedCards());
//             deck.shuffleDeck();
//
//             // Deal needed community cards
//             vector<Card> simCommunity = communityCards;
//             int cardsToDeal = neededCommunityCards();
//             for (int c = 0; c < cardsToDeal; ++c) {
//                 if (deck.cards.empty()) break; // Safety check
//                 simCommunity.push_back(deck.cards.back());
//                 deck.cards.pop_back();
//             }
//
//             // Evaluate hands using hash tables
//             vector<Card> p1Total = player1Hand;
//             p1Total.insert(p1Total.end(), simCommunity.begin(), simCommunity.end());
//             vector<Card> p2Total = player2Hand;
//             p2Total.insert(p2Total.end(), simCommunity.begin(), simCommunity.end());
//
//             HandValue hv1 = evaluator.evaluateHandHash(p1Total);
//             HandValue hv2 = evaluator.evaluateHandHash(p2Total);
//
//             // Compare hands
//             if (hv1 > hv2) p1Wins++;
//             else if (hv2 > hv1) p2Wins++;
//             else ties++;
//         }
//
//         auto endTime = chrono::high_resolution_clock::now();
//         execTime = chrono::duration_cast<chrono::milliseconds>(endTime - startTime).count();
//
//         p1Win = (p1Wins / static_cast<double>(trials)) * 100.0;
//         p2Win = (p2Wins / static_cast<double>(trials)) * 100.0;
//         tie = (ties / static_cast<double>(trials)) * 100.0;
//     }
// };
//
// // Function to generate a random card not already used
// Card generateRandomCard(unordered_set<string>& usedCards, mt19937& rng) {
//     while (true) {
//         int suitInt = rng() % 4; // 0 to 3
//         int rankInt = rng() % 13 + 2; // 2 to 14 (ACE)
//         Card card(static_cast<Suit>(suitInt), static_cast<Rank>(rankInt));
//         string cardKey = cardToString(card);
//         if (usedCards.find(cardKey) == usedCards.end()) {
//             usedCards.insert(cardKey);
//             return card;
//         }
//     }
// }
//
// // Function to generate a random hand for a player
// void generateRandomHand(vector<Card>& hand, unordered_set<string>& usedCards, mt19937& rng) {
//     hand.clear();
//     for (int i = 0; i < 2; ++i) {
//         hand.push_back(generateRandomCard(usedCards, rng));
//     }
// }
//
// // Function to randomly select a game stage
// string selectRandomGameStage(mt19937& rng) {
//     vector<string> stages = { "preflop", "flop", "turn", "river" };
//     return stages[rng() % stages.size()];
// }
//
// // Function to generate community cards based on game stage
// void generateCommunityCards(const string& stage, vector<Card>& communityCards, unordered_set<string>& usedCards, mt19937& rng) {
//     communityCards.clear();
//     int numCommunity = 0;
//     if (stage == "flop") numCommunity = 3;
//     else if (stage == "turn") numCommunity = 4;
//     else if (stage == "river") numCommunity = 5;
//
//     for (int i = 0; i < numCommunity; ++i) {
//         communityCards.push_back(generateRandomCard(usedCards, rng));
//     }
// }
//
// // Main function
// int main() {
//     cout << "=== Automated Poker Odds Simulator ===\n\n";
//
//     // Number of simulations to generate
//     int numSimulations = 100000; // Adjust as needed
//     int trialsPerSimulation = 100; // Number of Monte Carlo trials per simulation
//
//     // Open CSV file for writing
//     ofstream csvFile("PokerOddsDataset.csv");
//     if (!csvFile.is_open()) {
//         cerr << "Failed to open CSV file for writing." << endl;
//         return 1;
//     }
//
//     // Write CSV headers
//     csvFile << "SimulationID,Player1Hand,Player2Hand,GameStage,CommunityCards,"
//         << "P1Win_Map,P2Win_Map,Tie_Map,Time_Map,"
//         << "P1Win_Hash,P2Win_Hash,Tie_Hash,Time_Hash\n";
//
//     // Random number generator
//     random_device rd;
//     mt19937 rng(rd());
//
//     for (int simID = 1; simID <= numSimulations; ++simID) {
//         vector<Card> player1Hand;
//         vector<Card> player2Hand;
//         vector<Card> communityCards;
//         unordered_set<string> usedCards;
//
//         // Generate random hands for players
//         generateRandomHand(player1Hand, usedCards, rng);
//         generateRandomHand(player2Hand, usedCards, rng);
//
//         // Randomly select game stage
//         string gameStage = selectRandomGameStage(rng);
//
//         // Generate community cards based on game stage
//         generateCommunityCards(gameStage, communityCards, usedCards, rng);
//
//         // Initialize Simulator
//         Simulator simulator(player1Hand, player2Hand, gameStage, communityCards);
//
//         // Run map-based simulation
//         double p1WinMap = 0.0, p2WinMap = 0.0, tieMap = 0.0;
//         long long execTimeMap = 0;
//         simulator.runSimulationMap(trialsPerSimulation, p1WinMap, p2WinMap, tieMap, execTimeMap);
//
//         // Run hash table-based simulation
//         double p1WinHash = 0.0, p2WinHash = 0.0, tieHash = 0.0;
//         long long execTimeHash = 0;
//         simulator.runSimulationHash(trialsPerSimulation, p1WinHash, p2WinHash, tieHash, execTimeHash);
//
//         // Prepare data for CSV
//         stringstream ss;
//         ss << simID << ",";
//         // Player 1 Hand
//         ss << "\"";
//         for (const auto& card : player1Hand) ss << cardToString(card) << " ";
//         ss << "\",";
//         // Player 2 Hand
//         ss << "\"";
//         for (const auto& card : player2Hand) ss << cardToString(card) << " ";
//         ss << "\",";
//         // Game Stage
//         ss << gameStage << ",";
//         // Community Cards
//         ss << "\"";
//         for (const auto& card : communityCards) ss << cardToString(card) << " ";
//         ss << "\",";
//         // Map-Based Results
//         ss << p1WinMap << "," << p2WinMap << "," << tieMap << "," << execTimeMap << ",";
//         // Hash Table-Based Results
//         ss << p1WinHash << "," << p2WinHash << "," << tieHash << "," << execTimeHash << "\n";
//
//         // Write to CSV
//         csvFile << ss.str();
//
//         // Optional: Print progress to console
//         cout << "Simulation " << simID << " completed.\n";
//     }
//
//     csvFile.close();
//     cout << "\nAll simulations completed. Results saved to 'PokerOddsDataset.csv'.\n";
//
//     return 0;
// }