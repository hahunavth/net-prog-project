module com.netprog.client {
    requires javafx.controls;
    requires javafx.fxml;
    requires javafx.web;

    requires org.controlsfx.controls;
    requires com.dlsc.formsfx;
//    requires validatorfx;
    requires org.kordamp.ikonli.javafx;
    requires org.kordamp.bootstrapfx.core;
//    requires eu.hansolo.tilesfx;

    opens com.netprog.client to javafx.fxml;
    exports com.netprog.client;
    exports com.netprog.client.Experiment;
    opens com.netprog.client.Experiment to javafx.fxml;
}