//https://www.elitepvpers.com/forum/shaiya-pserver-guides-releases/3596037-release-call-psmagent-commands-via-sql.html
using System;
using System.Data.SqlTypes;
using Microsoft.SqlServer.Server;
using System.Text;
using System.IO;
using System.Net.Sockets;

public partial class Console
{
    [Microsoft.SqlServer.Server.SqlProcedure]
    public static void Command(SqlString argv)
    {
        byte[] buffer = new byte[1024];
        SqlPipe client = SqlContext.Pipe;
        Socket sender = null;
        string output = "0";

        try
        {
            sender = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            sender.Connect("127.0.0.1", 40900);

            var temp = new BinaryWriter(new MemoryStream(258));
            var binary = new BinaryWriter(new MemoryStream());

            string service = "ps_game";
            string command = argv.ToString();

            temp.Write((short)1281);
            temp.Write(Encoding.ASCII.GetBytes(service.ToString()));

            byte[] data = new byte[258];

            var stream = (MemoryStream)temp.BaseStream;
            stream.Position = 0;
            stream.Read(data, 0, (int)stream.Length);

            binary.BaseStream.Position = 0;
            short size = (short)(2 + data.Length + 2 + command.Length);
            binary.Write(size);
            binary.Write(data);
            binary.Write((short)command.Length);
            binary.Write(Encoding.ASCII.GetBytes(command));

            byte[] packet = ((MemoryStream)binary.BaseStream).ToArray();
            sender.Send(packet);
            sender.Receive(buffer);
            sender.Shutdown(SocketShutdown.Both);

        }
        catch (SocketException error)
        {
            output = string.Format("{0}", error.ToString());
        }
        catch (Exception error)
        {
            output = string.Format("{0}", error.ToString());
        }
        finally
        {
            sender.Close();
            client.Send(output);
        }
    }
}
