./cutechess-cli \
-engine name="" cmd="" proto=uci \
-engine name="" cmd="" proto=uci \
-each \
    st=0.2 timemargin=20 \
    -openings file= format=pgn order=sequential plies=16 \
    -games 2 -rounds 2500 -repeat 2 -maxmoves 200 \
-sprt elo0=0 elo1=10 alpha=0.05 beta=0.05 \
-ratinginterval 10 \
-pgnout "" \
-debug > sprt_