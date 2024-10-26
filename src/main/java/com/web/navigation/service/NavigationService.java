package com.web.navigation.service;


import org.springframework.stereotype.Service;

import com.web.navigation.entity.DataToSend;

@Service
public interface NavigationService {

    String executeNavigation(DataToSend startCors,DataToSend endCors);

    
}
