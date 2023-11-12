using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Security.Cryptography;


namespace ConsoleApp2
{
    class Program
    {
        static void Main(string[] args)
        {
            var p = new Program();
            Console.Write("start");
            Console.WriteLine(p.Decrypt("6zRdeCzU2lYXd9guASZH34g7m4tjYK7lm4SsHYE+ZhGhXWwWhr8wIfjlvZ8SpUBQqwjbE3IvcxhfxeTOsL9MKd+56W3cbAy0TgQ1damz+CX4fArong0+Zty0zwYA3S1p", "The S3cr3t Flag?"));
            Console.WriteLine("end");
        }

        public byte[] B64Decode(string b64Str)
        {
            try
            {
                byte[] byteArray = Encoding.UTF8.GetBytes(b64Str);
                return byteArray;
            }
            catch
            {
                // Handle exceptions or return null/empty array as needed
                return null;
            }
        }
        public string Decrypt(string ciphertext, string aesKey)
        {
            byte[] aesKeyBytes = B64Decode(aesKey);
            byte[] ivBytes = Encoding.UTF8.GetBytes("ILikeHardcodedIV");
            byte[] cipherBytes = B64Decode(ciphertext);

            // ...

            // Assuming aesKeyBytes, ivBytes, and cipherBytes are already populated byte arrays:
            using (Aes aesAlg = Aes.Create())
            {
                aesAlg.Key = aesKeyBytes;
                aesAlg.IV = ivBytes;
                // The default mode for `System.Security.Cryptography.Aes` is CBC, and the default padding is PKCS7.

                // Create a decryptor to perform the stream transform.
                ICryptoTransform aesDec = aesAlg.CreateDecryptor(aesAlg.Key, aesAlg.IV);

                // Create the streams used for decryption.
                byte[] plainBytes;
                using (MemoryStream msDecrypt = new MemoryStream(cipherBytes))
                {
                    using (CryptoStream csDecrypt = new CryptoStream(msDecrypt, aesDec, CryptoStreamMode.Read))
                    {
                        using (MemoryStream ms = new MemoryStream())
                        {
                            csDecrypt.CopyTo(ms);
                            plainBytes = ms.ToArray();
                        }
                    }
                }

                // Now `plainBytes` contains the decrypted data
                return Encoding.UTF8.GetString(plainBytes);
            }

        }

    }
}
