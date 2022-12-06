package com.netprog.client;

import javafx.fxml.FXML;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextField;

public class HelloController {
    @FXML
    private Button msgSend;

    @FXML
    private TextField msgTextbox;

    @FXML
    protected void onPressSendMsg() {
        System.out.println(msgTextbox.getText());
        HelloApplication.c.send(msgTextbox.getText());
    }
}