Entail LHS(root0,ll0,tr0,root1,ll1,tr1,root2,ll2,tr2,root3,ll3,tr3,root4,ll4,tr4,root5,ll5,tr5,root6,ll6,tr6,root7,ll7,tr7,root8,ll8,tr8,root9,ll9,tr9,root10,ll10,tr10,root11,ll11,tr11,root12,ll12,tr12,root13,ll13,tr13,root14,ll14,tr14,root15,ll15,tr15,root16,ll16,tr16,root17,ll17,tr17,root18,ll18,tr18,root19,ll19,tr19) |- RHS(root0,ll0,tr0,root1,ll1,tr1,root2,ll2,tr2,root3,ll3,tr3,root4,ll4,tr4,root5,ll5,tr5,root6,ll6,tr6,root7,ll7,tr7,root8,ll8,tr8,root9,ll9,tr9,root10,ll10,tr10,root11,ll11,tr11,root12,ll12,tr12,root13,ll13,tr13,root14,ll14,tr14,root15,ll15,tr15,root16,ll16,tr16,root17,ll17,tr17,root18,ll18,tr18,root19,ll19,tr19)


LHS(root0,ll0,tr0,root1,ll1,tr1,root2,ll2,tr2,root3,ll3,tr3,root4,ll4,tr4,root5,ll5,tr5,root6,ll6,tr6,root7,ll7,tr7,root8,ll8,tr8,root9,ll9,tr9,root10,ll10,tr10,root11,ll11,tr11,root12,ll12,tr12,root13,ll13,tr13,root14,ll14,tr14,root15,ll15,tr15,root16,ll16,tr16,root17,ll17,tr17,root18,ll18,tr18,root19,ll19,tr19) ::= TLL_tail(root0,nil,ll0,tr0,root1) * TLL_tail(root1,tr0,ll1,tr1,root2) * TLL_tail(root2,tr1,ll2,tr2,root3) * TLL_tail(root3,tr2,ll3,tr3,root4) * TLL_tail(root4,tr3,ll4,tr4,root5) * TLL_tail(root5,tr4,ll5,tr5,root6) * TLL_tail(root6,tr5,ll6,tr6,root7) * TLL_tail(root7,tr6,ll7,tr7,root8) * TLL_tail(root8,tr7,ll8,tr8,root9) * TLL_tail(root9,tr8,ll9,tr9,root10) * TLL_tail(root10,tr9,ll10,tr10,root11) * TLL_tail(root11,tr10,ll11,tr11,root12) * TLL_tail(root12,tr11,ll12,tr12,root13) * TLL_tail(root13,tr12,ll13,tr13,root14) * TLL_tail(root14,tr13,ll14,tr14,root15) * TLL_tail(root15,tr14,ll15,tr15,root16) * TLL_tail(root16,tr15,ll16,tr16,root17) * TLL_tail(root17,tr16,ll17,tr17,root18) * TLL_tail(root18,tr17,ll18,tr18,root19) * TLL_tail(root19,tr18,ll19,tr19,nil)

RHS(root0,ll0,tr0,root1,ll1,tr1,root2,ll2,tr2,root3,ll3,tr3,root4,ll4,tr4,root5,ll5,tr5,root6,ll6,tr6,root7,ll7,tr7,root8,ll8,tr8,root9,ll9,tr9,root10,ll10,tr10,root11,ll11,tr11,root12,ll12,tr12,root13,ll13,tr13,root14,ll14,tr14,root15,ll15,tr15,root16,ll16,tr16,root17,ll17,tr17,root18,ll18,tr18,root19,ll19,tr19) ::= \E root0,ll0,tr0,root1,ll1,tr1,root2,ll2,tr2,root3,ll3,tr3,root4,ll4,tr4,root5,ll5,tr5,root6,ll6,tr6,root7,ll7,tr7,root8,ll8,tr8,root9,ll9,tr9,root10,ll10,tr10,root11,ll11,tr11,root12,ll12,tr12,root13,ll13,tr13,root14,ll14,tr14,root15,ll15,tr15,root16,ll16,tr16,root17,ll17,tr17,root18,ll18,tr18,root19,ll19,tr19 . TLL_tail(root0,nil,ll0,tr0,root1) * TLL_tail(root2,tr1,ll2,tr2,root3) * TLL_tail(root5,tr4,ll5,tr5,root6) * TLL_tail(root8,tr7,ll8,tr8,root9) * TLL_tail(root10,tr9,ll10,tr10,root11) * TLL_tail(root7,tr6,ll7,tr7,root8) * TLL_tail(root9,tr8,ll9,tr9,root10) * TLL_tail(root4,tr3,ll4,tr4,root5) * TLL_tail(root13,tr12,ll13,tr13,root14) * TLL_tail(root11,tr10,ll11,tr11,root12) * TLL_tail(root15,tr14,ll15,tr15,root16) * TLL_tail(root12,tr11,ll12,tr12,root13) * TLL_tail(root17,tr16,ll17,tr17,root18) * TLL_tail(root14,tr13,ll14,tr14,root15) * TLL_tail(root6,tr5,ll6,tr6,root7) * TLL_tail(root19,tr18,ll19,tr19,nil) * TLL_tail(root1,tr0,ll1,tr1,root2) * TLL_tail(root16,tr15,ll16,tr16,root17) * TLL_tail(root3,tr2,ll3,tr3,root4) * TLL_tail(root18,tr17,ll18,tr18,root19)

TLL_plus(root,par,ll,lr) ::=  root->nil,nil,par,lr & root=ll |
  \E l,r,z . root->l,r,par,nil * TLL_plus(l,root,ll,z) * TLL_plus(r,root,z,lr)

TLL_tail(root,par,ll,tr,lr) ::=  root->nil,nil,par,lr & root=ll & root=tr |
  \E l,r,z . root->l,r,par,nil * TLL_plus(l,root,ll,z) * TLL_tail(r,root,z,tr,lr)
