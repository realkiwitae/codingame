# Spring-challenge-2023-ants
###

# resume
hexagonal grid boardgame

# wood 2 strategie: 
checking starter AI

# wood 1:
We can now create more ants collecting eggs.
1. place all the cells on the map using cube coordinates so we can use the notion of distance
2. fixed starter AI was enough to pass Bronze

# bronze:
1. for each base, map nearest egg
2. then map anything within half the base ressources

-- not enough to pass silver
changing strategy to:
1. graph all eggs on our side of the map with shortest path recursively
2. grab until done
3. same with crystals but map all crystals on the map

-- seed=-6995843859582959000