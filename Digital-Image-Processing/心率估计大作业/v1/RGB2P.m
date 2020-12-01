function P = RGB2P(R_value, G_value, B_value, Method)
    global numFrames;

    Smooth_R_value = smoothdata(R_value, Method);
    Smooth_G_value = smoothdata(G_value, Method);
    Smooth_B_value = smoothdata(B_value, Method);

    Avg_Smooth_R_value = sum(Smooth_R_value) / numFrames;
    Avg_Smooth_G_value = sum(Smooth_G_value) / numFrames;
    Avg_Smooth_B_value = sum(Smooth_B_value) / numFrames;    
    
    Normal_R_value = Smooth_R_value ./ Avg_Smooth_R_value;
    Normal_G_value = Smooth_G_value ./ Avg_Smooth_G_value;    
    Normal_B_value = Smooth_B_value ./ Avg_Smooth_B_value;

    Avg_Normal_R_value = sum(Normal_R_value) / numFrames; 
    Avg_Normal_G_value = sum(Normal_G_value) / numFrames; 
    Avg_Normal_B_value = sum(Normal_B_value) / numFrames; 

    S1 = 3 * Normal_R_value - 2 * Avg_Normal_G_value;
    S2 = 1.5 * Normal_R_value + Avg_Normal_G_value - 1.5 * Normal_B_value;

    % S1 = 3 * Normal_R_value - 2 * Normal_G_value;
    % S2 = 1.5 * Normal_R_value + Normal_G_value - 1.5 * Normal_B_value;

    Var_S1 = var(S1, 1);
    Var_S2 = var(S2, 1);
    alp = Var_S1 / Var_S2;
    
    P = S1 - alp * S2;
end