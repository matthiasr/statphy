BEGIN   {N_uni = 0; N_imp = 0 }

/^Uniform/  { pi_uni[N_uni] = $3; N_uni++; }

/^Importance/   { pi_imp[N_imp] = $3; N_imp++; }

END { avg_uni=0;avg_imp=0;var_uni=0;var_imp=0;
    for(i=0;i<N_uni;i++) avg_uni+=pi_uni[i];
    avg_uni/=N_uni;
    for(i=0;i<N_imp;i++) avg_imp+=pi_imp[i];
    avg_imp/=N_imp;

    for(i=0;i<N_uni;i++) var_uni+=(pi_uni[i]-avg_uni)^2;
    var_uni/=N_uni;
    for(i=0;i<N_imp;i++) var_imp+=(pi_imp[i]-avg_imp)^2;
    var_imp/=N_imp;

    printf "Uniform Sampling: %f +- %f\n", avg_uni, sqrt(var_uni);
    printf "Importance Sampling: %f +- %f\n", avg_imp, sqrt(var_imp); }
