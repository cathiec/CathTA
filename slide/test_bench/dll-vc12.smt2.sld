Entail LHS(x_emp,w_emp,u_emp,y_emp,z_emp) |- RHS(x_emp,w_emp,u_emp,y_emp,z_emp)


LHS(x_emp,w_emp,u_emp,y_emp,z_emp) ::= x_emp->w_emp,nil * y_emp->z_emp & x_emp!=w_emp & w_emp!=y_emp * dll(w_emp,u_emp,x_emp,y_emp)

RHS(x_emp,w_emp,u_emp,y_emp,z_emp) ::= dll(x_emp,y_emp,nil,z_emp)

dll(fr,bk,pr,nx) ::=  fr=nx & bk=pr & emp |
  \E u . fr->u,pr & fr!=nx & bk!=pr * dll(u,bk,fr,nx)
