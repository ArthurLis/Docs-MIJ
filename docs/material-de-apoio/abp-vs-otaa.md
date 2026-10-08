# ABP x OTAA

## ABP

A conexão ABP (Activation by Personalization) é uma conexão com características de funcionamento mais rápido, não depende uma troca de mensagens inicial, além de ser extremamente menos complexa. Porém, sua taxa de segurança, pelos mesmos motivos é muito menor quando comparada com a conexão OTAA, por suas chaves serem permamentes e estáticas.

É ideal para comunicações rápidas, ou conexões de teste.

## OTAA

A conexão OTAA (Over-the-Air Activation) é uma conexão com caraterísticas de funcionamento mais seguros, depende de uma troca de mensagens inicial, conhecido como join, por isso se torna mais complexa. Em compensação, as chaves não são estáticas, além de sempre expirarem, o que diminui o risco de cópia.

É ideal para conexões mais complexas.