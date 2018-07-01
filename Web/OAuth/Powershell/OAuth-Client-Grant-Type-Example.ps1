# Powershell - OAuth Example - Client Credential Grant Type
# Based on LinkedIn Learning: Web Security: OAuth and OpenID Connect: https://www.linkedin.com/learning/web-security-oauth-and-openid-connect/lab-build-an-example-with-the-command-line-and-postman
# PHP-Based League OAuth Server: https://oauth2.thephpleague.com/
# Json Web Token

$data = "grant_type=client_credentials&client_id=myawesomeapp&client_secret=abc123&scope=basic email"

try
{
    $response = Invoke-WebRequest -Uri http://localhost:4444/client_credentials.php/access_token -Method Post -Headers @{Accept = "1.0"} -Body $data -ContentType 'application/x-www-form-urlencoded'
    Write-Output $response

    $jsonWebToken = $response.Content | ConvertFrom-Json
    Write-Output "Json Web Token (JWT):"
    Write-Output $jsonWebToken

    $accessToken = $jsonWebToken.access_token
    Write-Output "Access Token:"
    Write-Output $accessToken
}
catch
{
    Write-Error "Failure '$_.Exception.Response.StatusDescription' requesting OAuth Client Credential Grant Type" -ErrorId $_.Exception.Response.StatusCode.value__
}


<# Json Web Token (JWT):
 
 token_type   : Bearer
 expires_in   : 3600
 access_token : eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsImp0aSI6IjA3ZjgyZTIwMGNhNjIzMjI2NTNhNWQ1MDc2ZjUwNWVmYmEwZTE5YzYwNzUyMTQzZWQ4MTJhYjc3MzA1MGQ3
                NjM4MWZjY2VkMDViMzU1NGZkIn0.eyJhdWQiOiJteWF3ZXNvbWVhcHAiLCJqdGkiOiIwN2Y4MmUyMDBjYTYyMzIyNjUzYTVkNTA3NmY1MDVlZmJhMGUxOWM2MDc1MjE0
                M2VkODEyYWI3NzMwNTBkNzYzODFmY2NlZDA1YjM1NTRmZCIsImlhdCI6MTUyNjE4Mzg0MywibmJmIjoxNTI2MTgzODQzLCJleHAiOjE1MjYxODc0NDMsInN1YiI6IiIs
                InNjb3BlcyI6WyJiYXNpYyIsImVtYWlsIl19.K6JbdX7jydVWkOO8txAuEv7mpBb33mgsdhq0CYbfsCznbYKdtnPnmf8412wz5EZHBwwxLzxFzZo9CunFvVejIJeW0nV
                s7xNWensCBCy08JbLHahrfb6LOLfd7aQJv0mqrfeSo5ZTY5Iopz3dY990XBkT1JnkGQ5qHsh3SQntxRilRqlDW5zZAtZU7Z5FfyOnxjXGmOCeBos9ZRypkaK6YbM16kq
                u6YB0sQ9JHm-PdOnP94WqvwW1sXZIOqduh2N1FwADWs0dno2A6wv3QexR1QuanyUteVt4VdHlNn2g_IZ8au5tMniFYg-CTBfnsuPJa38WBpIsRtDN6_XzTT5A0Q
#>

<# Access Token:
eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiIsImp0aSI6IjA3ZjgyZTIwMGNhNjIzMjI2NTNhNWQ1MDc2ZjUwNWVmYmEwZTE5YzYwNzUyMTQzZWQ4MTJhYjc3MzA1MGQ3NjM4MWZjY2VkMDVi
MzU1NGZkIn0.eyJhdWQiOiJteWF3ZXNvbWVhcHAiLCJqdGkiOiIwN2Y4MmUyMDBjYTYyMzIyNjUzYTVkNTA3NmY1MDVlZmJhMGUxOWM2MDc1MjE0M2VkODEyYWI3NzMwNTBkNzYzODFmY2Nl
ZDA1YjM1NTRmZCIsImlhdCI6MTUyNjE4Mzg0MywibmJmIjoxNTI2MTgzODQzLCJleHAiOjE1MjYxODc0NDMsInN1YiI6IiIsInNjb3BlcyI6WyJiYXNpYyIsImVtYWlsIl19.K6JbdX7jydV
WkOO8txAuEv7mpBb33mgsdhq0CYbfsCznbYKdtnPnmf8412wz5EZHBwwxLzxFzZo9CunFvVejIJeW0nVs7xNWensCBCy08JbLHahrfb6LOLfd7aQJv0mqrfeSo5ZTY5Iopz3dY990XBkT1Jn
kGQ5qHsh3SQntxRilRqlDW5zZAtZU7Z5FfyOnxjXGmOCeBos9ZRypkaK6YbM16kqu6YB0sQ9JHm-PdOnP94WqvwW1sXZIOqduh2N1FwADWs0dno2A6wv3QexR1QuanyUteVt4VdHlNn2g_IZ
8au5tMniFYg-CTBfnsuPJa38WBpIsRtDN6_XzTT5A0Q
#>


<# https://www.jsonwebtoken.io/
Header
{
 "typ": "JWT",
 "alg": "HS256"
}
#>


<# Payload
{
 "aud": "myawesomeapp",
 "jti": "07f82e200ca62322653a5d5076f505efba0e19c60752143ed812ab773050d76381fcced05b3554fd",
 "iat": 1526183843,
 "nbf": 1526183843,
 "exp": 1526187487,
 "sub": "",
 "scopes": [
  "basic",
  "email"
 ]
}
#>