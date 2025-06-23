import javafx.application.Application;
import javafx.geometry.*;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import javafx.scene.text.*;
import javafx.stage.Stage;
import javafx.animation.*;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;

public class FaceRecognitionAttendanceSystem extends Application {

    Label clockLabel = new Label();

    @Override
    public void start(Stage primaryStage) {
        primaryStage.setTitle("Face Recognition Based Attendance System");

        // Title
        Text title = new Text("Face Recognition Based Attendance System");
        title.setFont(Font.font("Arial", FontWeight.BOLD, 24));
        title.setFill(Color.WHITE);
        HBox titleBox = new HBox(title);
        titleBox.setAlignment(Pos.CENTER);
        titleBox.setPadding(new Insets(10));
        titleBox.setStyle("-fx-background-color: black;");

        // Clock
        clockLabel.setFont(Font.font("Verdana", FontWeight.BOLD, 18));
        clockLabel.setTextFill(Color.ORANGE);
        updateClock();
        Timeline timeline = new Timeline(new KeyFrame(javafx.util.Duration.seconds(1), e -> updateClock()));
        timeline.setCycleCount(Animation.INDEFINITE);
        timeline.play();

        VBox clockBox = new VBox(clockLabel);
        clockBox.setAlignment(Pos.CENTER);

        // Left Panel (For Already Registered)
        Label registeredLabel = new Label("For Already Registered");
        registeredLabel.setFont(Font.font("Arial", FontWeight.BOLD, 16));
        registeredLabel.setStyle("-fx-background-color: green; -fx-text-fill: white;");
        registeredLabel.setMaxWidth(Double.MAX_VALUE);
        registeredLabel.setAlignment(Pos.CENTER);

        Button takeAttendanceBtn = new Button("Take Attendance");
        takeAttendanceBtn.setMaxWidth(Double.MAX_VALUE);
        takeAttendanceBtn.setStyle("-fx-background-color: yellow; -fx-font-weight: bold;");

        TableView<String> attendanceTable = new TableView<>();
        attendanceTable.setPrefHeight(250);
        attendanceTable.setPlaceholder(new Label("No Attendance Yet"));

        Button quitBtn = new Button("Quit");
        quitBtn.setStyle("-fx-background-color: red; -fx-text-fill: white;");
        quitBtn.setMaxWidth(Double.MAX_VALUE);

        VBox leftPane = new VBox(10, registeredLabel, takeAttendanceBtn, attendanceTable, quitBtn);
        leftPane.setPadding(new Insets(10));
        leftPane.setStyle("-fx-background-color: #b2f7ef;");
        leftPane.setPrefWidth(400);

        // Right Panel (For New Registration)
        Label newRegLabel = new Label("For New Registration");
        newRegLabel.setFont(Font.font("Arial", FontWeight.BOLD, 16));
        newRegLabel.setStyle("-fx-background-color: red; -fx-text-fill: white;");
        newRegLabel.setMaxWidth(Double.MAX_VALUE);
        newRegLabel.setAlignment(Pos.CENTER);

        TextField idField = new TextField();
        idField.setPromptText("Enter ID");
        Button clearIdBtn = new Button("Clear");

        TextField nameField = new TextField();
        nameField.setPromptText("Enter Name");
        Button clearNameBtn = new Button("Clear");

        Label infoLabel = new Label("1)Take Images  -> 2)Save Profile");
        infoLabel.setTextFill(Color.DARKBLUE);

        Button takeImagesBtn = new Button("Take Images");
        takeImagesBtn.setStyle("-fx-background-color: blue; -fx-text-fill: white;");
        takeImagesBtn.setMaxWidth(Double.MAX_VALUE);

        Button saveProfileBtn = new Button("Save Profile");
        saveProfileBtn.setStyle("-fx-background-color: blue; -fx-text-fill: white;");
        saveProfileBtn.setMaxWidth(Double.MAX_VALUE);

        Label totalRegLabel = new Label("Total Registered Users = 0");
        totalRegLabel.setTextFill(Color.MAGENTA);

        GridPane formGrid = new GridPane();
        formGrid.setVgap(10);
        formGrid.setHgap(10);
        formGrid.setPadding(new Insets(10));
        formGrid.addRow(0, idField, clearIdBtn);
        formGrid.addRow(1, nameField, clearNameBtn);

        VBox rightPane = new VBox(10, newRegLabel, formGrid, infoLabel, takeImagesBtn, saveProfileBtn, totalRegLabel);
        rightPane.setPadding(new Insets(10));
        rightPane.setStyle("-fx-background-color: #d9faff;");
        rightPane.setPrefWidth(400);

        // Main layout
        HBox content = new HBox(20, leftPane, rightPane);
        content.setPadding(new Insets(10));
        VBox root = new VBox(10, titleBox, clockBox, content);
        root.setStyle("-fx-background-color: lightgray;");

        Scene scene = new Scene(root, 850, 600);
        primaryStage.setScene(scene);
        primaryStage.show();
    }

    private void updateClock() {
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("dd-MMM-yyyy | HH:mm:ss");
        clockLabel.setText(formatter.format(LocalTime.now()));
    }

    public static void main(String[] args) {
        launch(args);
    }
}
