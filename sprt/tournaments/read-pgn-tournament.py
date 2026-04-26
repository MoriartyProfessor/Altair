import chess.pgn
from collections import defaultdict

def calculate_tournament_scores(pgn_filename):
    scores = defaultdict(float)
    games_count = defaultdict(int)

    with open(pgn_filename, "r") as pgn_file:
        while True:
            game = chess.pgn.read_game(pgn_file)
            if game is None:
                break  # End of file

            white = game.headers.get("White", "Unknown")
            black = game.headers.get("Black", "Unknown")
            result = game.headers.get("Result", "*")

            games_count[white] += 1
            games_count[black] += 1

            if result == "1-0":
                scores[white] += 1.0
            elif result == "0-1":
                scores[black] += 1.0
            elif result == "1/2-1/2":
                scores[white] += 0.5
                scores[black] += 0.5

    standings = sorted(scores.items(), key=lambda x: x[1], reverse=True)

    print(f"{'Player':<25} | {'Score':<6} | {'Games':<5}")
    print("-" * 45)
    for player, score in standings:
        print(f"{player:<25} | {score:<6} | {games_count[player]:<5}")

calculate_tournament_scores("")