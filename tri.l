entier $tab[ 10 ];

initialiser()
{
    $tab[0] = 12;    $tab[1] = 67;    $tab[2] = 9;
    $tab[3] = 92;    $tab[8] = 44;    $tab[5] = 27;
    $tab[6] = 32;    $tab[7] = 12;    $tab[8] = 43;
    $tab[9] = 5;    
}

afficher()
{
    ecrire( $tab[0] );    ecrire( $tab[1] );    ecrire( $tab[2] );
    ecrire( $tab[3] );    ecrire( $tab[4] );    ecrire( $tab[5] );
    ecrire( $tab[6] );    ecrire( $tab[7] );    ecrire( $tab[8] );
    ecrire( $tab[9] );
    ecrire( 0 ); # marqueur fin de tableau         
}

echanger( entier $i, entier $j )
entier $temp;
{
    $temp = $tab[ $j ];
    $tab[ $j ] = $tab[ $i ];
    $tab[ $i ] = $temp;
}

trier( entier $n )
entier $echange, entier $j, entier $m;
{
    $m = $n;
    $echange = 1;
    tantque $echange = 1 faire
    {
        $echange = 0;
        $j = 0;
        tantque $j < $m - 1 faire 
        {
            si $tab[ $j + 1 ] < $tab[ $j ] alors {
                echanger( $j, $j + 1 );
                $echange = 1;
            }
            $j = $j + 1;
        }
        $m = $m - 1;        
    }    
}

main()
{
    initialiser();
    afficher();
    trier( 10 );
    #afficher();
}
