Entail LHS(x,p) |- RHS(x,p)

LHS(x,p) ::= \E y . points_to(x,y) * LS(y,p)

RHS(x,p) ::= \E q . LS(x,q) * points_to(q,p)

points_to(x,y) ::= x->y

LS(hd,tl) ::= hd=tl & emp | \E z . hd->z * LS(z,tl)
