main()
entier $a;
{
    $a = lire();
    # affiche $a si <= 50 et pair
    si ( !($a < 50) | 50 = $a ) & ( $a - ( $a / 2 ) * 2 = 0 ) alors 
    {
        ecrire( $a );
    }
    sinon
    {
        ecrire( 0-1 );
    }
}
