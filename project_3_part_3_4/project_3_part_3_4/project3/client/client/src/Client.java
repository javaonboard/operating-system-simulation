import java.net.*;
import java.io.*;
import java.util.Scanner;

public class Client {


    private Socket clientSocket;
    private OutputStream out;
    private BufferedReader in;


    public static void main(String args[]) throws IOException {
        Client client = new Client();
        Scanner sc = new Scanner(System.in);
        System.out.println("please enter the ip eg. 127.0.0.1");
        String ip = sc.nextLine();
        System.out.println("please enter the server port eg. 21234");
        String port = sc.nextLine();
        client.startConnection(ip, Integer.parseInt(port));
        System.out.println("your now connected to the server!");
        System.out.println("please enter the file that you want to submit. (ensure file exist in the same directory as this program)");
        String filename = sc.nextLine();
        File file = new File(filename);
        byte[] bytes = new byte[16 * 1024];
        InputStream input = new FileInputStream(file);
        String response = client.sendMessage(bytes,input);
        System.out.println(response);

    }

    public void startConnection(String ip, int port) throws IOException {
        clientSocket = new Socket(ip, port);
        out = clientSocket.getOutputStream();
        in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
    }

    public String sendMessage(byte[] bytes, InputStream input) throws IOException {
        int count;
        Scanner sc = new Scanner(System.in);
        String cl;
        String sr;
        while(true){
            System.out.println("server message...");
            String resp = in.readLine();
            System.out.println(resp);
            cl = sc.nextLine();
            out.write(cl.getBytes(),0,cl.length());
            sr = in.readLine();
            System.out.println(sr);
            if(sr.equalsIgnoreCase("please send your file...")) {
                while ((count = input.read(bytes)) > 0) {
                    out.write(bytes, 0, count);
                }
            } else if(sr.equalsIgnoreCase("bye"))break;
            else {
                System.out.println(resp);
             }
            }

            String resp = in.readLine();
            stopConnection();
            return resp;
    }

    public void stopConnection() throws IOException {
        in.close();
        out.close();
        clientSocket.close();
    }

}
