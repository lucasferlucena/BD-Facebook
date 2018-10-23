#include <iostream>
#include <windows.h>
#include <mysql.h>
#include <string>
#include "funcoes.h"
#include "telas.h"

int main()
{

    //conexao com a database
    conn = mysql_init(0);
    if(conn)
        cout << "";
    else
        cout << "connection object problem" << mysql_error(conn) << endl;

    conn = mysql_real_connect(conn, "127.0.0.1", "root", "", "facebook", 3306, NULL, 0);


    //funçoes

    //cadastrarUsuario();
    //criarGrupo();
    //efetuarLogin();
    //buscarPessoas();
    //buscarGrupos();

    telaInicial();

    mysql_close(conn);
    return 0;
}
