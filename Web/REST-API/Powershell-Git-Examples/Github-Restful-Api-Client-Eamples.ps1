## REST Powershell
## Github RESTful API Examples
## Create and use a personal access token: https://github.com/settings/tokens

[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12

$username = "Enter-name-of-authenticated-user"
$token = "Enter-authenticated-user-token-or-password"
$userToken = [Convert]::ToBase64String([Text.Encoding]::ASCII.GetBytes(("{0}" -f $token)))
$userrepo = "Enter-repository-user-name"
$reponame = "Enter-existing-repository-name"

$createRepo = "Enter-repository-name-to-be-created"
$postRepoBody = @{name=$createRepo}
$postRepoJson = $postRepoBody | ConvertTo-Json

try
{
    $response = Invoke-RestMethod https://api.github.com -Method Get -Headers @{Authorization = "Basic $userToken"}
    Write-Output $response
}
catch
{
    Write-Error "Failure '$_.Exception.Response.StatusDescription' connecting to Restful Github" -ErrorId $_.Exception.Response.StatusCode.value__
}

try
{
    $response = Invoke-RestMethod https://api.github.com/user/repos -Method Get -Headers @{Authorization = "Basic $userToken"}
    Write-Output $response
}
catch
{
    Write-Error "Failure '$_.Exception.Response.StatusDescription' getting authenticated user '$username' Github repositories" -ErrorId $_.Exception.Response.StatusCode.value__
}

try
{
    $response = Invoke-RestMethod https://api.github.com/repos/$userrepo/$reponame -Method Get -Headers @{Authorization = "Basic $userToken"}
    Write-Output $response
}
catch
{
    Write-Error "Failure '$_.Exception.Response.StatusDescription' getting user '$userrepo' repository '$reponame'" -ErrorId $_.Exception.Response.StatusCode.value__
}

try
{
    $response = Invoke-RestMethod https://api.github.com/repos/$userrepo/$reponame/subscription -Method Get -Headers @{Authorization = "Basic $userToken"}
    Write-Output $response
}
catch
{
    Write-Error "Failure '$_.Exception.Response.StatusDescription' getting user '$userrepo' repository '$reponame' subscriptions" -ErrorId $_.Exception.Response.StatusCode.value__
}

try
{
    $response = Invoke-RestMethod https://api.github.com/repos/$userrepo/$reponame/subscribers -Method Get -Headers @{Authorization = "Basic $userToken"}
    Write-Output $response
}
catch
{
    Write-Error "Failure '$_.Exception.Response.StatusDescription' getting user '$userrepo' repository '$reponame' subscribers" -ErrorId $_.Exception.Response.StatusCode.value__
}

try
{
    $response = Invoke-RestMethod https://api.github.com/user/repos -Method Post -Headers @{Authorization = "Basic $userToken"} -Body $postRepoJson -ContentType 'application/json'
    Write-Output $response
}
catch
{
    Write-Error "Failure '$_.Exception.Response.StatusDescription' creating authenticated user '$username' Github repository '$createRepo'" -ErrorId $_.Exception.Response.StatusCode.value__
}
